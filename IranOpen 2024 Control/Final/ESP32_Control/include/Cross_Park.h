#ifndef CROSS_PARK
#define CROSS_PARK

#include "Motion.h"
#if MODE == CITY

class Cross_Park : public Motion
{
private:
    bool direction;
    bool angle_memory_reset_status_cross = true;
    
    int sign_distance = 0;
    int back_distance = 0;
    int front_distance = 0;
    int crosstrack_error = 0;
    int counter = 0;
    int block_counter = -1;
    int left_vl = 0;
    int angle_memory_cross = 0;
    int traveled_distance = 0;
    int temp_traveled_distance = 0;
    
    double base_angle = 0;
    double exit_angle = 0; 
    
    void (Cross_Park::*current_instruction)();
    void sign_verification();
    void start_to_park();
    void check_block();
    void park_preparation();
    void park_I();
    void park_II();
    void exit_I();
    void exit_II();
    void end_parking();
    int park_obstacle_controller(bool direction, int speed, int sensitivity);
    int min_of_back_VLs();
    int min_of_front_VLs();
    int go_straight_cross(double gain = 5);

public:
    void instruction();
    Cross_Park();
};
#if !defined(NO_GLOBAL_PARALLEL_PARK)
extern Cross_Park Cross;
#endif
#endif

#endif