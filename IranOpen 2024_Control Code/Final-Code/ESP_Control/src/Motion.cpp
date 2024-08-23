#include "Motion.h"

Motion motion;

int counter = 0;
timer_config_t motionmodel_timer_config;
TaskHandle_t motionmodel_task;

void IRAM_ATTR motionmodel_timer_isr(void *)
{
    xTaskResumeFromISR(motionmodel_task);
    timer_init(TIMER_GROUP_1, TIMER_0, &motionmodel_timer_config);
}

void motionmodel_updater(void *)
{
    vTaskSuspend(NULL);
    while (true)
    {
        if ((counter % 2) == 0)
        {
            // motion.calculate_theta();
            // motion.calculate_theta_temp();
            // motion.calculate_motion_model_parameters();
            // motion.calculate_motion_model_parameters_temp();
            //motion.print_MM_data();
            vTaskDelay(2) ; 
        }
        else if ((counter % 9) == 0)
        {
            // motion.calculate_curveture();
            vTaskDelay(2) ; 
            if (counter > 1000)
            {
                counter = 0;
            }
        }
        counter++;
        vTaskSuspend(motionmodel_task);
    }
}

void Motion::updater_setup()
{
    xTaskCreatePinnedToCore(
        motionmodel_updater,
        "motion model updater",
        2048,
        NULL,
        15,
        &motionmodel_task,
        APP_CPU_NUM);
    motionmodel_timer_config.divider = 128;
    motionmodel_timer_config.counter_dir = TIMER_COUNT_UP;
    motionmodel_timer_config.counter_en = TIMER_START;
    motionmodel_timer_config.alarm_en = TIMER_ALARM_EN;
    motionmodel_timer_config.auto_reload = TIMER_AUTORELOAD_EN;
    timer_init(TIMER_GROUP_1, TIMER_0, &motionmodel_timer_config);
    timer_set_counter_value(TIMER_GROUP_1, TIMER_0, 0);
    timer_set_alarm_value(TIMER_GROUP_1, TIMER_0, 50 * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_1, TIMER_0);
    timer_pause(TIMER_GROUP_1, TIMER_0);
    timer_isr_register(TIMER_GROUP_1, TIMER_0, motionmodel_timer_isr, nullptr, ESP_INTR_FLAG_IRAM, nullptr);
}

//General

void Motion::operate(int _steer, int _speed)
{
    if (_steer > 100)
    {
        _steer = 100;
    }
    else if (_steer < -100)
    {
        _steer = -100;
    }
    else
    {
        _steer = _steer;
    }

    if (_speed > 100)
    {
        _speed = 100;
    }
    else if (_speed < -100)
    {
        _speed = -100;
    }

    steer.turn(_steer);
    motor.UART_accelerate(_speed);
}

void Motion::stop()
{
    operate(0, 0);
}

bool Motion::go_for_distance(int distance, int _steer, int _speed)
{
    if (distance_memory_reset_status == true)
    {
        motor.start_encode();
        last_enc = 0.0;
        delta_enc = 0.0;
        last_enc_temp = 0.0;
        delta_enc_temp = 0.0;
        last_enc_curve = 0.0;
        distance_memory_reset_status = false;
        // cmps.set_saved_cmps(cmps.get_car_angle());
        return false;
    }
    if (motor.get_distance_covered_mm() < distance)
    {
        operate(_steer, _speed);
        return false;
    }
    else
    {
        distance_memory_reset_status = true;
        return true;
    }
}

bool Motion::go_for_angle(int angle, int _steer, int _speed)
{
    if (angle_memory_reset_status == true)
    {
        angle_memory = cmps.get_car_angle();
        angle_memory_reset_status = false;
        return false;
    }
    else
    {
        traveled_angle = not_abs(cmps.get_car_angle() - angle_memory);
        if (traveled_angle > 180)
        {
            traveled_angle -= 360;
        }
        if (not_abs(traveled_angle) <= angle)
        {
            operate(_steer, _speed);
            return false;
        }
        else
        {
            angle_memory_reset_status = true;
            return true;
        }
    }
}

bool Motion::go_to_xy(double des_x, double des_y, int speed){
    calculat_back_state_local();
    double error = 0;
    double kp = 4.5;
    double kd = 0.7;
    double kpp = 0.5;
    if(go_to_xy_flag){
        last_distance = distance = sqrt(pow(des_x - x_local, 2) + pow(des_y - y_local, 2));
        go_to_xy_flag = false;
    }
    else{
        last_distance = distance ; 
        distance = sqrt(pow(des_x - x_local, 2) + pow(des_y - y_local, 2));
    }

    if(distance < 75 || (distance - last_distance > 0)){ //80 - 300
        reset_localization_local();
        go_to_xy_flag = true;
        return true;
    }
    else{
        if(des_x - x_local > 0){
            error = (atan((des_y - y_local) / (des_x - x_local)) * 180 / M_PI) - theta_local;
        }
        else if(des_x - x_local <= 0 && des_y - y_local > 0){
            error = (atan((des_y - y_local) / (des_x - x_local)) * 180 / M_PI) + 180 - theta_local;
        }
        else if(des_x - x_local <= 0 && des_y - y_local < 0){
            error = (atan((des_y - y_local) / (des_x - x_local)) * 180 / M_PI) - 180 - theta_local;
        }        
        
        if(error > 180){
            error = 360 - error;
        }
        else if(error < -180){
            error = 360 + error;
        }

        //stanley:
            // int steer = 57.29 *  atan((20* (error - 55) / (Ks + (0.93 * speed) )));

        //pure_persuit:
            // int steer = 57.29 * atan((2 * error * L) / ((Kpp * speed) * (Kpp * speed)));
        
        //PID:
            int steer = (kp * error) + (kd * (error - last_error_xy));
            last_error_xy = error;

        motion.operate(steer , speed);

        vTaskDelay(2) ; 

        return false;
    }
}

bool Motion::go_to_xytheta(double des_x, double des_y, double des_theta, int speed){
    calculat_back_state_local();
    double a = tan(convert_angle_to_radian(des_theta));
    double b = des_y - (a * des_x);
    double crosstrack_error = ((a * x_local) + (-1 * y_local) + b ) / (sqrt((a * a) + 1));
    double heading_error = des_theta - theta_local;
    double kp = 0.1;
    double kd = 3;
    double kh = 2.7;
    double k = 0.8;//0.5
    double distance = sqrt(pow(des_x - x_local, 2) + pow(des_y - y_local, 2));

    //PID:
        int steer = (kp * crosstrack_error) + (kd * heading_error);

    //stanley:
        // int steer = (kh * heading_error) + (57.29 *  atan((k * crosstrack_error) / (Ks + (0.93 * speed) )));

    if(distance < 100 && heading_error < 10){
        return true;
    }
    else{
        motion.operate(steer, speed_controller(speed));
        return false;
    }
    
}

double Motion::convert_angle_to_radian(double angle){
    return (angle * (M_PI / 180));
}

int Motion::min_of_front_VLs(){
    vl.start_vl(VF | VFML | VFMR | VFR | VFL);
    int min_distance = min(vl.get_data(vl.FRONT_MID_LEFT), vl.get_data(vl.FRONT));
    min_distance = min(min_distance, vl.get_data(vl.FRONT_MID_RIGHT));
    min_distance = min(min_distance, 3 * vl.get_data(vl.FRONT_LEFT));   // edit: x3 to reduce the impact of FRONT_LEFT  race: comment front_left and front_right
    min_distance = min(min_distance, 3 * vl.get_data(vl.FRONT_RIGHT));  // x3 to reduce the impact of FRONT_RIGHT

    return min_distance;
}

int Motion::min_of_back_VLs(){
    vl.start_vl(VB | VBML | VBMR | VBR | VBL);
    int min_distance = min(vl.get_data(vl.BACK_MID_LEFT), vl.get_data(vl.BACK));
    min_distance = min(min_distance, vl.get_data(vl.BACK_MID_RIGHT));
    min_distance = min(min_distance, 3 * vl.get_data(vl.BACK_LEFT));   // x3 to reduce the impact of FRONT_LEFT
    min_distance = min(min_distance, 3 * vl.get_data(vl.BACK_RIGHT));  // x3 to reduce the impact of FRONT_RIGHT

    return min_distance;
}

void Motion::calculate_kappa(){
    time_kappa = millis(); 
    if((time_kappa - last_time_kappa >= 500) && motor.get_current_speed() != 0){
        length = motor.get_distance_covered_mm() - first_point;
        theta_kappa = cmps.get_car_angle() - first_angle;
        first_angle = cmps.get_car_angle();
        first_point += length;

        if(theta_kappa > 180){
            theta_kappa = theta_kappa - 360;
        }
        else if(theta_kappa < -180){
            theta_kappa = theta_kappa + 360;
        }

        if(theta_kappa / length <= 1 && theta_kappa / length >= -1){
            kappa = theta_kappa / length;  
            for(int i = 13; i >= 0; i--){
                kappa_arr[i + 1] = kappa_arr[i];
            }
            kappa_arr[0] = kappa;
        }
        last_time_kappa = millis();
    }
}

double Motion::mean_kappa(){
    double sum = 0;
    double sum_std = 0;
    int n = 15;

    for(int i = 0; i < n; i++){
        sum += kappa_arr[i];
    }

    double mean = sum / 15.0;
    for(int i = 0; i < n; i++){
        sum_std += pow((kappa_arr[i] - mean),2);
    }

    double std = sqrt(sum_std / n);
    double filtered_mean = sum / 15.0;
    for(int i = 0; i < 15; i++){
        if(kappa_arr[i] < (mean - std) || kappa_arr[i] > (mean + std)){
            n--;
            if(n == 0){
                filtered_mean = kappa_arr[0];
            }
            else{
                sum = sum - kappa_arr[i];
                filtered_mean = sum / n;
            }
        }
    }
    return filtered_mean;
}

// Controllers

int Motion::PID(){
    sum_CTE -= I_buffer[19];
    for(int i = 19; i > 0; i--){
        I_buffer[i] = I_buffer[i - 1];
    }
    I_buffer[0] = vision.get_distance_from_midline();
    sum_CTE += I_buffer[0];
    
    double P = Kp * vision.get_distance_from_midline();
    double I = Ki * sum_CTE;
    double D = Kd * vision.get_angle();
    
    int steer = -(P + D + I);

    vTaskDelay(2); 
    return steer;
}

int Motion::pure_pursuit(){
    int crosstrack_error = -vision.get_distance_from_midline();
    int speed = motor.get_current_speed();

    int steer = 57.29 * atan((2 * crosstrack_error * L) / ((Kpp * speed) * (Kpp * speed)));

    vTaskDelay(2);
    return steer;
}

int Motion::stanley(){
    int speed = motor.get_current_speed();
    int heading_error = -vision.get_angle();

    // sum_angle -= angle_arr[4];
    // for(int i = 4; i > 0; i--){
    //     angle_arr[i] = angle_arr[i - 1];                 
    // }   
    // angle_arr[0] = -vision.get_angle();  
    // sum_angle += angle_arr[0];
    // heading_error = sum_angle / 5.0;

    //heading error filter
    // if(not_abs(heading_error - last_HE) > 30){
    //     if(counter_HE > 2){
    //         last_HE = heading_error;
    //         counter_HE = 0;
    //     }
    //     heading_error = last_HE;
    // }
    // last_HE = heading_error;

    int crosstrack_error = -vision.get_distance_from_midline();

    // sum_distance -= distance_arr[4];
    // for(int i = 4; i > 0; i--){
    //     distance_arr[i] = distance_arr[i - 1];                 
    // }   
    // distance_arr[0] = -vision.get_distance_from_midline();  
    // sum_distance += distance_arr[0];
    // crosstrack_error = sum_distance / 5.0;

    //crosstrack error filter
    // if(not_abs(crosstrack_error - last_CTE) > 50){
    //     if(counter_CTE > 2){
    //         last_CTE = crosstrack_error;
    //         counter_CTE = 0;
    //     }
    //     crosstrack_error = last_CTE;
    //     counter_CTE++;
    // }
    // last_CTE = crosstrack_error;

    int steer = 0;
    int n = 5;
    double a = 0;
    double b = 0;
    
    // crosstrack_error = crosstrack_error + ( 0.13 * sum_HE);    //0.13    // shift for race 
    // crosstrack_error = crosstrack_error - ( 0.13 * sum_HE);    //edit
    
    a = K_HE * heading_error;
    b = 57.29 *  atan((K * crosstrack_error) / (Ks + (0.93 * speed) ));
    steer = (K_HE * heading_error) + (57.29 *  atan((K * crosstrack_error) / (Ks + (0.93 * speed) )));

    //line data filter

    // if(reset_filter){   
    //     last_steer = steer;
    //     reset_filter = false;
    // }
    // if(not_abs(steer - last_steer) > 40){   //edit: race
    //     steer = last_steer;
    // }
    // last_steer = steer;
        
    vTaskDelay(2);
    return steer;
}

// int Motion::speed_controller(int max_speed){    //edit: race: disable front_left and front_right VLs
//     int obstacle_distance = min_of_front_VLs();
    
//     if(obstacle_distance < 400){    //check obstacle    //edit: race: 250   city: 400    
//         if(obstacle_distance > 150){
//             double P_speed = 0.1;
//             real_speed = P_speed * obstacle_distance;
//             if(real_speed < 10){
//                 speed = real_speed = 10;
//             }
//         }
//         else{
//             speed = real_speed = 5; // edit :  race: 
//         }
//     }
//     else{                 
//         sum_HE -= angle_buffer[6];
//         for(int i = 6; i > 0; i--){
//             angle_buffer[i] = angle_buffer[i - 1];                 
//         }   
//         angle_buffer[0] = -vision.get_angle();  //edit: race: not_abs   
//         sum_HE += angle_buffer[0];

//         real_speed = max_speed - (not_abs(sum_HE) * gain);   //set speed 

//         if(real_speed < 10){
//             speed = real_speed = 10;
//         }
//         if(real_speed - motor.get_current_speed() > 1 ){     //acceleration 
//             counter_speed_controller += 1;
//             if(counter_speed_controller > 3){
//                 counter_speed_controller = 0;
//                 speed = motor.get_current_speed() + 1;
//             }
//         }
//         else if(real_speed - motor.get_current_speed() < -1){
//             speed = motor.get_current_speed() - 1;
//         }
//     }

//     if(real_speed > max_speed){
//         speed = max_speed;
//     }
//     vTaskDelay(2);
//     return speed;
// }

int Motion::speed_controller(int max_speed){    //edit: race: disable front_left and front_right VLs
    int obstacle_distance = min_of_front_VLs();
    
    if(obstacle_distance < 400){    //check obstacle    //edit: race: 250   city: 400    
        if(obstacle_distance > 150){
            double P_speed = 0.1;
            speed = P_speed * obstacle_distance;
            if(speed < 10){
                speed = 10;
            }
        }
        else{
            speed = 0;
        }
    }
    else{                 
        sum_HE -= angle_buffer[6];
        for(int i = 6; i > 0; i--){
            angle_buffer[i] = angle_buffer[i - 1];                 
        }   
        angle_buffer[0] = -vision.get_angle();  //edit: race: not_abs   
        sum_HE += angle_buffer[0];

        speed = max_speed - (not_abs(sum_HE) * gain);   //set speed 

        if(speed < 10){
            speed = 10;
        }
        if(speed - motor.get_current_speed() > 1 ){     //acceleration 
            speed = motor.get_current_speed() + 1;
        }
        else if(speed - motor.get_current_speed() < -1){
            speed = motor.get_current_speed() - 1;
        }
    }

    if(speed > max_speed){
        speed = max_speed;
    }

    vTaskDelay(2);
    return speed;
}

int Motion::obstacle_controller(int max_speed){
    int obstacle_distance = 0;
    if(max_speed >= 0){
        obstacle_distance = min_of_front_VLs();
        if(obstacle_distance < 400){    //check obstacle
            if(obstacle_distance > 150){    //150
            double P_obstacle = 0.1;
                speed_obs = P_obstacle * obstacle_distance;
                if(speed_obs < 10){
                    speed_obs = 10;
                }
            }
            else{
                speed_obs = 0;
            }
        }
        else{
            speed_obs = max_speed;
        }
        if(speed_obs > max_speed){
            speed_obs = max_speed;
        }
    }
    else{
        obstacle_distance = min_of_back_VLs();
        double P_obstacle = -0.1;
        if(obstacle_distance < 500){    //check obstacle
            if(obstacle_distance > 100){    //150
                speed_obs = P_obstacle * obstacle_distance;
                if(speed_obs < -10){
                    speed_obs = -10;
                }
            }
            else{
                speed_obs = 0;
            }
        }
        else{
            speed_obs = max_speed;
        }
        if(speed_obs < max_speed){
            speed_obs = max_speed;
        }
    }

    if(speed_obs - motor.get_current_speed() > 2 ){     //acceleration 
        speed_obs = motor.get_current_speed() + 2;
    }
    else if(speed_obs - motor.get_current_speed() < -2){
        speed_obs = motor.get_current_speed() - 2;
    }

    vTaskDelay(2);
    return speed_obs;
}

int Motion::go_straight(double gain){
    int steer = 0;
    if (angle_memory_reset_status){
        angle_memory = cmps.get_car_angle();
        angle_memory_reset_status = false;
    }
    else{
        double current_angle = cmps.get_car_angle();
        double error = current_angle - angle_memory;
        if(error > 180){
            error = error - 360;
        }
        else if(error < -180){
            error = error + 360;
        }

        steer = error * gain;
    }
    return steer;
}

//Localization
void Motion::calculat_back_state_global(){
    double dx = 0;
    double dy = 0;
    double d_theta = 0;
    int speed = motor.get_current_speed();

    if(start_flag_global == true){     // set base theta and base time
        base_theta_global = cmps.get_car_angle();
        last_time_global, time_global = millis();
        start_flag_global = false;
    }

    time_global = millis();

    if(time_global - last_time_global >= 10){    //set frequency of operation
        dt_global = (time_global - last_time_global);
        dt_global = dt_global / 1000;
        theta_global = -(cmps.get_car_angle() - base_theta_global);
        if(theta_global > 180){
            theta_global = theta_global - 360;
        }
        else if(theta_global < -180){
            theta_global = theta_global + 360;
        }

        dx = speed * Kt_global * cos(theta_global / 57.29) * dt_global;
        dy = speed * Kt_global * sin(theta_global / 57.29) * dt_global;
        x_global += dx;
        y_global += dy;

        last_time_global = time_global ; 

        vTaskDelay(2);
    }
}

void Motion::calculat_front_state_global(){
    double dx = 0;
    double dy = 0;
    double d_theta = 0;
    int speed = motor.get_current_speed();

    double delta = map(steer.get_current_steer(), -100, 100, -30, 30);

    if(start_flag_global == true){     // set base theta and base time
        base_theta_global = cmps.get_car_angle();
        last_time_global, time_global = millis();
        start_flag_global = false;
    }

    time_global = millis();

    if(time_global - last_time_global >= 10){    //set frequency of operation
        dt_global = (time_global - last_time_global);
        dt_global = dt_global / 1000;
        theta_global = -(cmps.get_car_angle() - base_theta_global);

        if(theta_global > 180){
            theta_global = theta_global - 360;
        }
        else if(theta_global < -180){
            theta_global = theta_global + 360;
        }

        dx = speed * Kt_global * cos((theta_global + delta) / 57.29) * dt_global;
        dy = speed * Kt_global * sin((theta_global + delta) / 57.29) * dt_global;
        x_global += dx;
        y_global += dy;

        last_time_global = time_global ; 

        vTaskDelay(2);
    }
}

//Remember to set start_flag_local true at the end of usage
void Motion::calculat_back_state_local(){   
    double dx,dy,d_theta = 0;
    int speed = motor.get_current_speed();

    if(start_flag_local == true){     // set base theta and base time
        base_theta_local = cmps.get_car_angle();
        last_time_local, time_local = millis();
        x_local = 0;
        y_local = 0;
        theta_local = 0;
        start_flag_local = false;
    }

    time_local = millis();

    if(time_local - last_time_local >= 10){    //set frequency of operation
        dt_local = (time_local - last_time_local);
        dt_local = dt_local / 1000;
        theta_local = -(cmps.get_car_angle() - base_theta_local);

        if(theta_local > 180){
            theta_local = theta_local - 360;
        }
        else if(theta_local < -180){
            theta_local = theta_local + 360;
        }


        dx = speed * Kt_local * cos(theta_local / 57.29) * dt_local;
        dy = speed * Kt_local * sin(theta_local / 57.29) * dt_local;
        x_local += dx;
        y_local += dy;

        last_time_local = time_local ; 
    }
}

//Remember to set start_flag_local true at the end of usage
void Motion::calculat_front_state_local(){
    double dx,dy,d_theta = 0;
    int speed = motor.get_current_speed();

    double delta = map(steer.get_current_steer() , -100, 100, -30, 30);

    if(start_flag_local == true){     // set base theta and base time
        base_theta_local = cmps.get_car_angle();
        last_time_local = time_local = millis();
        x_local = y_local = theta_local = 0;
        start_flag_local = false;
    }

    time_local = millis();
    
    if(time_local - last_time_local >= 10){    //set frequency of operation
        dt_local = (time_local - last_time_local);
        dt_local = dt_local / 1000.0;
        theta_local = -(cmps.get_car_angle() - base_theta_local);
        
        if(theta_local > 180){
            theta_local = theta_local - 360;
        }
        else if(theta_local < -180){
            theta_local = theta_local + 360;
        }

        dx = speed * Kt_local * cos((theta_local + delta) / 57.29) * dt_local;
        dy = speed * Kt_local * sin((theta_local + delta) / 57.29) * dt_local;
        x_local += dx;
        y_local += dy;

        last_time_local = time_local; 
    }
}

void Motion::reset_localization_local(){
    start_flag_local = true;
}

void Motion::reset_stanley_filter(){
    reset_filter = true;
}
