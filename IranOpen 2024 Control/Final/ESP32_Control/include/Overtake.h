#ifndef _OVERTAKE_H_
#define _OVERTAKE_H_

#include "Modules.h"
#include "Actuator.h"
#include "Motion.h"

class Overtake : public Motion{
    private:
        bool direction;
        int side_detection_parameter = 1;
        void (Overtake::*current_instruction)();

    public:
        Overtake();
        void instruction();
        void obstacle_check();
        void turn_left();
        void overtaking();
        void right_obstacle_check();
        void turn_right();
        void end_overtake();

};
#if !defined(NO_GLOBAL_INTERSECTION)
extern Overtake overtake;
#endif
#endif