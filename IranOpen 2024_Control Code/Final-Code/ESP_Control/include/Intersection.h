#ifndef _INTERSECTION_H_
#define _INTERSECTION_H_

#include "Modules.h"
#include "Actuator.h"
#include "Motion.h"

class Intersection : public Motion{
    private:
        bool direction;
        bool getting = true;

        int sign;
        int sign_distance = 310;    //sign verification condition
        int crosswalk_angle = 0;
        int crosswalk_sum = 0;
        int crosswalk_arr[40] {0};
        int crosswalk_data_n = 0;
        int angle_correction_value = 0;
        int kappa_gain = 0;
        int traveled_distance = 0;
        int saved_distance = 0;
        int correction = 0;
        int preparation_value = 0;
        int line_direction_parameter = 1;   //right
        int side_detection_parameter = 1;
        int side_invalid_counter = 0;

        double alpha = 200;
        double x = 0;
        double y = 0;
        double L = 26.3;
        double dv = 0;
        double kappa = 0;

        void (Intersection::*current_instruction)();

    public:
        Intersection();
        void instruction();
        void sign_verification();
        void calculate_crosswalk_mean();
        void getting_close();
        void check_traffic_light();
        void set_preparation_value();
        void intersection_preparation();
        void angle_correction();
        void set_direction();
        void straight();
        void straight_II();
        void right_I();
        void right_II();
        void right_III();
        void right_IV();
        void ghazAl_correction();
        void left_I();
        void left_II();
        void side_correction();
        void end_intersection();
        int  mean_crosswalk_angle();
        void send_crossWalk_flag();
        void send_crossWalk_end_flag();
};
#if !defined(NO_GLOBAL_INTERSECTION)
extern Intersection intersection;
#endif
#endif