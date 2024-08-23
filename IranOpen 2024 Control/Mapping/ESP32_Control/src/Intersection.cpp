#include "Intersection.h"

#if MODE == CITY
Intersection intersection;

Intersection::Intersection() : direction(true), current_instruction(&Intersection::send_crossWalk_flag) 
{}

void Intersection::instruction(){
    (this->*current_instruction)();
    vTaskDelay(2); 
}

void Intersection::send_crossWalk_flag(){
    vl.start_vl(VB | VBL | VBML | VBMR | VBR);
    if(vision.get_distance_from_sign() < 600){
        for(int i = 0; i < 15; i++){
            uart.jetson_print('r');
        }
        current_instruction = &Intersection::sign_verification;
    }
    else{
        motion.operate(motion.stanley(), motion.obstacle_controller(10));
    }
}

void Intersection::sign_verification(){
    uart.debug_println("sign_verification: ");
    if(vision.get_distance_from_sign() != -10){
        sign_distance = vision.get_distance_from_sign();
    }
    if(sign_distance < 300 && sign_distance != -10){
        motion.stop();
        vTaskDelay(3000);
        if(vision.get_distance_from_sign() != -10){
            sign_distance = vision.get_distance_from_sign();
        } 
        motor.start_encode();
        if(crosswalk_data_n > 40){
            crosswalk_data_n = 40;
        }
        
        if(kappa >= 0){ //Right
            kappa_gain = 5700;
        }
        else{   //Left
            kappa_gain = 5200;
        }
        
        current_instruction = &Intersection::calculate_crosswalk_mean;
    }
    else{
        kappa = motion.mean_kappa();
        motion.operate(motion.stanley(), motion.obstacle_controller(10));
        if(vision.get_crosswalk_angle() != 90){
            getting = false;
            for(int i = 39; i > 0; i--){
                crosswalk_arr[i] = crosswalk_arr[i - 1];
            }
            crosswalk_arr[0] = vision.get_crosswalk_angle();
            crosswalk_data_n += 1;
        }

    }
}

void Intersection::calculate_crosswalk_mean(){
    uart.debug_println("calculate_crosswalk_mean: ");
    if(motor.get_distance_covered_mm() >= (sign_distance - 200) + (kappa_gain * kappa)){ 
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        motion.set_angle_memory_reset(true);
        if(crosswalk_data_n > 40){
            crosswalk_data_n = 40;
        }
        if(getting){
           current_instruction = &Intersection::getting_close; 
        }
        else{
            crosswalk_angle = mean_crosswalk_angle();
            current_instruction = &Intersection::check_traffic_light;
        }
    }
    else{
        motion.operate(motion.go_straight(8.1), motion.obstacle_controller(10));
        if(vision.get_crosswalk_angle() != 90){
            getting = false;
            for(int i = 39; i > 0; i--){
                crosswalk_arr[i] = crosswalk_arr[i - 1];
            }
            crosswalk_arr[0] = vision.get_crosswalk_angle();
            crosswalk_data_n += 1;
        }
    }
}

void Intersection::getting_close(){
    uart.debug_println("getting: ");
    int getting_steer = -(kappa * 2900);
    if(getting_steer > -30 && getting_steer < 30){
        getting_steer = 0;
    }
    if(motion.go_for_distance(75, 0, motion.obstacle_controller(20))){   //edit:  150
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        if(crosswalk_data_n > 40){
            crosswalk_data_n = 40;
        }
        if(crosswalk_data_n == 0){
            crosswalk_angle = 0;
        }
        else{
            crosswalk_angle = intersection.mean_crosswalk_angle();
        }
        current_instruction = &Intersection::check_traffic_light;
    }
    else{
        if(vision.get_crosswalk_angle() != 90){
            for(int i = 39; i > 0; i--){
                crosswalk_arr[i] = crosswalk_arr[i - 1];
            }
            crosswalk_arr[0] = vision.get_crosswalk_angle();
            crosswalk_data_n += 1;
        }
    }
}

void Intersection::check_traffic_light(){
    if(vision.get_traffic_light() == 11 || vision.get_traffic_light() == 19){
        current_instruction = &Intersection::set_preparation_value;
    }
    else{
        motion.stop();
        vTaskDelay(1000);
    }
}

void Intersection::set_preparation_value(){
    // if(kappa > 0 && crosswalk_angle > 0){
    //     uart.debug_println("1");
    //     preparation_value = kappa * 30000;
    // }
    // else if(kappa < 0 && crosswalk_angle <= 0){
    //     uart.debug_println("2");
    //     preparation_value = kappa * 2500;
    // }
    // else{
    uart.debug_println("3");
    preparation_value = (not_abs(kappa) * 2900);     //not_abs(kappa)
    // }
    uart.debug_println("cross: ");
    uart.debug_println(crosswalk_angle);
    uart.debug_println("kappa: ");
    uart.debug_println(kappa);
    current_instruction = &Intersection::intersection_preparation;
}

void Intersection::intersection_preparation(){
    uart.debug_println("intersection_preparation: ");
    if(motion.go_for_distance(400 - preparation_value, motion.go_straight(8.1), motion.obstacle_controller(20))){ //edit: a:
        uart.debug_print("preparation vlue: ");
        uart.debug_println(preparation_value);
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        motion.set_angle_memory_reset(true);
        if(crosswalk_angle > -3 && crosswalk_angle < 3){
            current_instruction = &Intersection::set_direction;
        }
        else{
            current_instruction = &Intersection::angle_correction;
        }
    }
}

void Intersection::angle_correction(){
    uart.debug_println("angle_correction: ");
    int steer = 0;
    correction = -1 * crosswalk_angle * 0.75;
    if(correction > 0){
        steer = 90;
    }
    else{
        steer = -90;
    }
    if(motion.go_for_angle(not_abs(correction), steer, motion.obstacle_controller(25))){
        uart.debug_print("correction: ");
        uart.debug_println(correction);
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        current_instruction = &Intersection::set_direction;
    }    
}

void Intersection::set_direction(){
    sign = vision.get_sign();
    if(sign == 10){ //straight
        motion.stop();
        vTaskDelay(500);
        line_direction_parameter = vision.get_line_direction();
        motor.start_encode();
        current_instruction = &Intersection::straight;
    }
    else if(sign == 8){ // right
        motor.start_encode();
        current_instruction = &Intersection::right_I;
    }
    else if(sign == 4){ //left
        motor.start_encode();
        if(not_abs(crosswalk_angle) > 55){
            current_instruction = &Intersection::ghazAl_correction;    
        }
        else{
            current_instruction = &Intersection::left_I;
        }
    }
}

void Intersection::straight(){  //edit:    a: 
    if(motion.go_for_distance(200, 15, motion.obstacle_controller(25))){
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        current_instruction = &Intersection::straight_II;
    }
}

void Intersection::straight_II(){
    if(line_direction_parameter == 0){
        if(motion.go_for_angle(10, -90, motion.obstacle_controller(25))){
            motion.stop();
            vTaskDelay(500);
            line_direction_parameter = vision.get_line_direction();
        }
    }
    else if(line_direction_parameter == 3){
        if(motion.go_for_distance(250, 0, motion.obstacle_controller(25))){ //edit:    a: 
            motion.stop();
            vTaskDelay(500);
            line_direction_parameter = vision.get_line_direction();
        }
        else{
            if(vision.get_line_direction() == 1){
                line_direction_parameter = vision.get_line_direction();
            }
        }
    }
    else{
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        side_detection_parameter = vision.get_side_detection();
        current_instruction = &Intersection::end_intersection;
    }
}

// void Intersection::right_I(){   //edit:    g:
//     if(go_for_distance(200 - crosswalk_angle, 0, motion.obstacle_controller(-25))){   //(not_abs(motion.get_kappa()) * 5000) //edit : angle correction shift
//         motion.stop();
//         vTaskDelay(500);
//         motion.reset_localization_local();
//         motor.start_encode();
//         current_instruction = &Intersection::right_II;
//     }
// }

// void Intersection::right_II(){
//     if(motion.go_for_angle(90, -90, motion.obstacle_controller(40))){  //go for angle or go for distance ???
//         motion.stop();
//         vTaskDelay(500);
//         motor.start_encode(); 
//         current_instruction = &Intersection::right_III;
//     }
// }

// void Intersection::right_III(){
//     if(motion.go_for_distance(250, 0, motion.obstacle_controller(-25))){  
//         motion.stop();
//         vTaskDelay(500);
//         motor.start_encode();
//         side_detection_parameter = vision.get_side_detection();
//         current_instruction = &Intersection::side_correction;
//     }
// }

void Intersection::right_I(){   //edit:    g:
uart.debug_println("right_I: ");
    if(motion.go_for_angle(50, -70, motion.obstacle_controller(40))){   //-90
        motion.stop();
        vTaskDelay(500);
        motor.start_encode(); 
        current_instruction = &Intersection::right_II;
    }
}

void Intersection::right_II(){
    uart.debug_println("right_II: ");
    if(motion.go_for_angle(15, 80, motion.obstacle_controller(-40))){   //-90
        motion.stop();
        vTaskDelay(500);
        motor.start_encode(); 
        current_instruction = &Intersection::right_III;
    }
}

void Intersection::right_III(){
    uart.debug_println("right_III: ");
    if(motion.go_for_angle(20, -85, motion.obstacle_controller(40))){
        motion.stop();
        vTaskDelay(500);
        motor.start_encode(); 
        current_instruction = &Intersection::side_correction;
    }
}

// void Intersection::right_IV(){
//     if(motion.go_for_distance(75, 0, motion.obstacle_controller(-25))){  //350
//         motion.stop();
//         vTaskDelay(500);
//         motor.start_encode();
//         side_detection_parameter = vision.get_side_detection();
//         current_instruction = &Intersection::side_correction;
//     }
// }

void Intersection::ghazAl_correction(){
    if(motion.go_for_distance(200, 0, motion.obstacle_controller(-25))){
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        current_instruction = &Intersection::left_I;
    }
}

void Intersection::left_I(){    //edit:    g:
    if(motion.go_for_distance(75 + crosswalk_angle, 0, motion.obstacle_controller(20))){  //edit : angle correction shift
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        current_instruction = &Intersection::left_II;
    }
}

void Intersection::left_II(){
    if(motion.go_for_angle(70 , 85, motion.obstacle_controller(40))){
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        side_detection_parameter = vision.get_side_detection();
        current_instruction = &Intersection::side_correction;
    }
}

void Intersection::side_correction(){
    if(side_detection_parameter == 0){
        if(motion.go_for_angle(33, -90, motion.obstacle_controller(25))){   //edit:    a:
            motion.stop();
            vTaskDelay(500);
            side_detection_parameter = vision.get_side_detection();
        }
        else{
            if(vision.get_side_detection() == 1){
                motion.stop();
                vTaskDelay(500);
                motion.set_angle_memory_reset(true);
                side_detection_parameter = vision.get_side_detection();
            }
        }
    }
    else if(side_detection_parameter == 2){
        if(side_invalid_counter < 2){
            if(motor.get_distance_covered_mm() > 100){  //edit: 150
                motion.stop();
                vTaskDelay(500);
                side_invalid_counter++;
                motor.start_encode();
                side_detection_parameter = vision.get_side_detection();
            }
            else{
                motion.operate(motion.stanley(), motion.obstacle_controller(15));
                if(vision.get_side_detection() == 1){
                    motion.stop();
                    vTaskDelay(500);
                    motor.start_encode();
                    side_detection_parameter = vision.get_side_detection();
                }
            }
        }
        else{
            current_instruction = &Intersection::end_intersection;
        }
    }
    else{
        current_instruction = &Intersection::end_intersection;
    }
}

void Intersection::end_intersection(){
    motion.stop();
    vTaskDelay(500);
    getting = true;
    crosswalk_data_n = 0;
    saved_distance = 0;
    line_direction_parameter = 1;
    side_detection_parameter = 1;
    side_invalid_counter = 0;
    vision.set_sign(vision.TRAVERSE);
    vision.set_distance_from_sign(0);
    motion.reset_kappa_parameter();
    motion.reset_localization_local();
    for(int i = 0; i < 15; i++){
        kappa_arr[i] = 0;
    }
    for(int i = 0; i < 40; i++){
        crosswalk_arr[i] = 0;
    }
    send_crossWalk_end_flag();
    motor.start_encode();
    vl.stop_vl(VB | VBL | VBML | VBMR | VBR);
    current_instruction = &Intersection::send_crossWalk_flag;
}

int Intersection::mean_crosswalk_angle(){
    int sum = 0;
    int sum_std = 0;
    int n = crosswalk_data_n;
    for(int i = 0; i < n; i++){
        sum += crosswalk_arr[i];
    }

    double mean = sum / crosswalk_data_n;
    for(int i = 0; i < n; i++){
        sum_std += pow((crosswalk_arr[i] - mean),2);
    }

    double std = sqrt(sum_std / n);
    int filtered_mean = sum / n;
    for(int i = 0; i < n; i++){
        if(crosswalk_arr[i] < (mean - std) || crosswalk_arr[i] > (mean + std)){
            n--;
            if(n == 0){
                filtered_mean = crosswalk_arr[0];
            }
            else{
                sum = sum - crosswalk_arr[i];
                filtered_mean = sum / n;
            }
        }
    }

    return filtered_mean;
}

void Intersection::send_crossWalk_end_flag(){
    for(int i = 0; i < 15; i++){
    uart.jetson_print('e');
  }
}

#endif