#include "Cross_Park.h"
#if MODE == CITY

Cross_Park Cross;

Cross_Park::Cross_Park() : direction(true), current_instruction(&Cross_Park::sign_verification)
{}

void Cross_Park::instruction(){
    (this->*current_instruction)();
    vTaskDelay(2); 
}

void Cross_Park::sign_verification(){
    vl.start_vl(VB | VBL | VBML | VBMR | VBR | VL);
    if(vision.get_distance_from_sign() > 400 && vision.get_distance_from_sign() < 600){     //edit:
        motion.stop();
        vTaskDelay(500);
        sign_distance = (vision.get_distance_from_sign()) + (not_abs(motion.get_kappa()) * 500);    //edit:  e:
        motor.start_encode();
        current_instruction = &Cross_Park::start_to_park;
    }
    else{
        motion.operate(motion.stanley(), motion.obstacle_controller(20));
    }
}

void Cross_Park::start_to_park(){
    if (motor.get_distance_covered_mm() < sign_distance + 75 + 100){    //edit: cover sign distance + VL + first_block
        motion.operate(motion.stanley(), motion.obstacle_controller(20));
    }
    else{
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        crosstrack_error = not_abs(vision.get_distance_from_midline());
        current_instruction = &Cross_Park::check_block;
    }
}

void Cross_Park::check_block(){ 
    if(motor.get_distance_covered_mm() < 300){  //edit: f:
        motion.operate(motion.stanley(), park_obstacle_controller(1, 20, 50));
        left_vl = vl.get_data(vl.LEFT);
        if(left_vl < (500 + 370 + 100 + (1.5 * vision.get_distance_from_midline()))){     //edit: Park_Box + width_of_road + VL_from_mid_line + ....    f:   a:
            counter++;
        }
    } 
    else{
        if (counter >= 30){
            motion.stop();  //edit
            vTaskDelay(500);
            counter = 0;
            block_counter++;
            crosstrack_error = not_abs(vision.get_distance_from_midline());
            motor.start_encode();
            current_instruction = &Cross_Park::check_block;
            if(block_counter == 2){
                current_instruction = &Cross_Park::end_parking;
            }
        }
        else{
            motor.start_encode();
            current_instruction = &Cross_Park::park_preparation;
        }
    }
}

// void Cross_Park::check_block(){
//     if(motor.get_distance_covered_mm() < 900){
//         motion.operate(motion.stanley(), park_obstacle_controller(1, 20 ,50));
//         left_vl = vl.get_data(vl.LEFT);
//         if(left_vl < (500 + 300 + 100 + (1.5 * vision.get_distance_from_midline()))){
//             counter++;
//         }
//         if(traveled_distance >= 300 && counter < 20){
//             motion.stop();
//             vTaskDelay(500);
//             motor.start_encode();
//             current_instruction = &Cross_Park::park_preparation;
//         }
//         else if(traveled_distance < 300){
//             traveled_distance = motor.get_distance_covered_mm() - temp_traveled_distance;
//         }
//         if(counter >= 20){
//             temp_traveled_distance = motor.get_distance_covered_mm();
//             counter = 0;
//         }
//     }
//     else{
//         motion.stop();
//         vTaskDelay(500);
//         motor.start_encode();
//         current_instruction = &Cross_Park::end_parking;
//     }
// }

void Cross_Park::park_preparation(){
    if(motor.get_distance_covered_mm() < 425){    // edit: rear wheel + preparation -> new : car ass
        motion.operate(motion.stanley(), park_obstacle_controller(1, 20, 50));
    }
    else{
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        current_instruction = &Cross_Park::park_I;
    }
}

void Cross_Park::park_I(){
    bool first_move = motion.go_for_distance(700, 95, park_obstacle_controller(0, 25, 50));
    if(first_move){
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        current_instruction = &Cross_Park::park_II;
    }
}

void Cross_Park::park_II(){
    bool first_move = motion.go_for_distance(500, 0, park_obstacle_controller(0, 40, 50));
    if(first_move){  //edit:   a:
        motion.stop();
        vTaskDelay(5000);
        motor.start_encode();
        angle_memory_reset_status_cross = true;
        current_instruction = &Cross_Park::exit_I;
    }
    // else{
    //     motion.stop();
    //     vTaskDelay(5000);
    //     motor.start_encode();
    //     angle_memory_reset_status_cross = true;
    //     current_instruction = &Cross_Park::exit_I;
    // }
}

void Cross_Park::exit_I(){
    bool first_move = motion.go_for_distance(350, 0, park_obstacle_controller(1, 40, 50));
    if(first_move){  //edit:   a:
        motion.stop();
        vTaskDelay(500);
        // motor.start_encode();
        // angle_memory_reset_status_cross = true;
        current_instruction = &Cross_Park::exit_II;
    }
    // if(motor.get_distance_covered_mm() < 300){  //edit: a:
    //     motion.operate(go_straight_cross(8.1), park_obstacle_controller(1, 40, 50));
    // }
    // else{
    //     motion.stop();
    //     vTaskDelay(500);
    //     current_instruction = &Cross_Park::exit_II;
    // }
}

void Cross_Park::exit_II(){
    bool first_move = motion.go_for_distance(750, 90, park_obstacle_controller(1, 25, 50));     //edit:
    if(first_move){
        motion.stop();
        vTaskDelay(500);
        motor.start_encode();
        current_instruction = &Cross_Park::end_parking;
    }
}

void Cross_Park::end_parking(){
    vision.set_sign(vision.TRAVERSE);
    motion.reset_kappa_parameter(); //delete
    for(int i = 0; i < 15; i++){
        kappa_arr[i] = 0;
    }
    vision.set_distance_from_sign(0);
    vl.stop_vl(VB | VBL | VBML | VBMR | VBR | VR);
    current_instruction = &Cross_Park::sign_verification;
}

int Cross_Park::min_of_back_VLs(){
    int back_distance = min(vl.get_data(vl.BACK_MID_LEFT), vl.get_data(vl.BACK));
    back_distance = min(back_distance, vl.get_data(vl.BACK_MID_RIGHT));

    return back_distance;
}

int Cross_Park::min_of_front_VLs(){
    int front_distance = min(vl.get_data(vl.FRONT_MID_LEFT), vl.get_data(vl.FRONT));
    front_distance = min(front_distance, vl.get_data(vl.FRONT_MID_RIGHT));
    front_distance = min(front_distance , 4 * vl.get_data(vl.FRONT_LEFT));

    return front_distance;
}

int Cross_Park::park_obstacle_controller(bool direction, int park_speed, int sensitivity){   //1:front   0:back
    int speed = 0;
    if(direction){
        int obstacle_distance = Cross.min_of_front_VLs();
        if(obstacle_distance < sensitivity){
            speed = 0;
        }
        else{
            speed = park_speed;
        }
    }
    else{
        int obstacle_distance = Cross.min_of_back_VLs();
        if(obstacle_distance < sensitivity){
            speed = 0;
        }
        else{
            speed = -park_speed;
        }
    }

    return speed;
}

int Cross_Park::go_straight_cross(double gain){
    int steer = 0;
    if(angle_memory_reset_status_cross){
        angle_memory_cross = cmps.get_car_angle();
        angle_memory_reset_status_cross = false;
    }
    else{
        double current_angle = cmps.get_car_angle();
        double error = current_angle - angle_memory_cross;
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

#endif