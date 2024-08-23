#ifndef PARALLEL_PARK
#define PARALLEL_PARK

#include "Motion.h"
#if MODE == CITY

class Parallel_Park : public Motion
{
private:
    bool direction;

    int sign_distance = 0;
    int back_distance = 0;
    int front_distance = 0;
    int crosstrack_error = 0;
    int line_direction_parameter = 1;
    int counter = 0;
    int block_counter = -1;
    int right_vl = 0;
    int traveled_distance = 0;
    int temp_traveled_distance = 0;
    
    double kappa = 0;
    double base_angle = 0;
    double exit_angle = 0;
    
    void (Parallel_Park::*current_instruction)();
    void sign_verification();
    void start_to_park();
    void check_block();
    void park_preparation();
    void park_I();
    void park_II();
    void park_III();
    void park_IV();
    void park_V();
    void exit_I();
    void exit_II();
    void exit_III();
    void exit_IV();
    void exit_V();
    void check_line();
    void end_parking();
    int park_speed_controller(bool direction, int speed, int sensitivity);
    int min_of_back_VLs();
    int min_of_front_VLs();

public:
    void instruction();
    Parallel_Park();
};
#if !defined(NO_GLOBAL_PARALLEL_PARK)
extern Parallel_Park Parallel;
#endif
#endif

#endif