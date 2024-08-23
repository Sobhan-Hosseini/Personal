#include "Overtake.h"

#if MODE == CITY
Overtake overtake;

Overtake::Overtake() : direction(true), current_instruction(&Overtake::obstacle_check) 
{}

void Overtake::instruction(){
    (this->*current_instruction)();
    // vTaskDelay(2); 
}

void Overtake::obstacle_check(){
    int obstacle_distance = motion.min_of_front_VLs();
    if(obstacle_distance < 300){
        motion.stop();
        // vTaskDelay(500);
        current_instruction = &Overtake::turn_left;
    }
    else{
        motion.operate(motion.stanley(), motion.speed_controller(motion.get_base_speed()));
    }
}

void Overtake::turn_left(){
    if(side_detection_parameter != 0){
        if(motion.go_for_angle(40, 80, 25)){
            motion.stop();
            vTaskDelay(250);
            side_detection_parameter = 0;
        }
        // else{
        //     if(vision.get_line_direction() == 0){
        //         motion.stop();
        //         // vTaskDelay(250);
        //         motion.set_angle_memory_reset(true);
        //         side_detection_parameter = vision.get_line_direction();
        //     }
        // }
    }
    else{
        // motion.stop();
        // vTaskDelay(250);
        motor.start_encode();
        current_instruction = &Overtake::overtaking;
    }
}

void Overtake::overtaking(){
    if(motor.get_distance_covered_mm() > 450){
        motion.stop();
        // vTaskDelay(250);
        motor.start_encode();
        vl.start_vl(VR);
        current_instruction = &Overtake::right_obstacle_check;
    }
    else{
        motion.operate(motion.stanley(), motion.speed_controller(motion.get_base_speed()));
    }
}

void Overtake::right_obstacle_check(){
    if(vl.get_data(vl.RIGHT) > 350 + (1.5 * vision.get_distance_from_midline())){   //edit : width_of_road + VL
        motion.stop();
        // vTaskDelay(250);
        motor.start_encode();
        side_detection_parameter = 0;
        current_instruction = &Overtake::turn_right;
    }
    else{
        motion.operate(motion.stanley(), motion.speed_controller(motion.get_base_speed()));
    }
}

void Overtake::turn_right(){
    if(side_detection_parameter != 1){
        if(motion.go_for_angle(20, -80, 25)){
            // motion.stop();
            // vTaskDelay(250);
            side_detection_parameter = vision.get_line_direction();
        }
        // else{
        //     if(vision.get_line_direction() == 1){
        //         motion.stop();
        //         // vTaskDelay(250);
        //         motion.set_angle_memory_reset(true);
        //         side_detection_parameter = vision.get_line_direction();
        //     }
        // }
    }
    else{
        motion.stop();
        // vTaskDelay(250);
        motor.start_encode();
        current_instruction = &Overtake::end_overtake;
    }
}

void Overtake::end_overtake(){
    motion.stop();
    vTaskDelay(250);
    motor.start_encode();
    vision.set_sign(vision.TRAVERSE);
    side_detection_parameter = 1;
    current_instruction = &Overtake::obstacle_check;
}
#endif