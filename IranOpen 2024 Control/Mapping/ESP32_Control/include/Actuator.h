#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Common.h"
#include "Motion.h"

#define SERVO_OUT 16
#define SERVO_MAX_RIGHT 100
#define SERVO_MAX_LEFT -100
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define TIMER_SCALE (APB_CLK_FREQ / 128000)

#if MODE == RACE
#define SPEEDSTEP   8
#else
#define SPEEDSTEP   10
#endif

typedef struct Coefficients
{
    double p_distance = 0.18,
           d_angle = 70,
           i_trajectory = 0.0,
#if MODE == CITY
           K_GLC = 1.25, // 1.16
        K_s = 20,        // 20
        K_HE = 0.5;      // 0.525
#else
           K_GLC = 2.9,
           K_s = 60,
           K_HE = 0.29;

// K_GLC = 2.796,     // 2.796
// K_s = 60,          // 60
// K_HE = 0.29;       // 0.29
#endif
} Coefficients;

void IRAM_ATTR distance_timer_isr(void *);

class Motor
{
    friend void IRAM_ATTR distance_timer_isr(void *);
public:
    Motor();
    void motor_setup();
    void UART_accelerate(int speed);
    void start_encode()
    {
        reset_value();
        distance = 0;
        timer_start(TIMER_GROUP_0, TIMER_0);
        encoder = true;
    }
    void stop_encode(){   
        timer_pause(TIMER_GROUP_0, TIMER_0);
        encoder = false;
    }
    double get_distance_covered_mm(){
        return (double)distance / distance_const;
    }
    int get_current_speed() { return current_speed ; }

    void set_distance_covered_mm(long input)
    {
        distance = input ; 
    }
    void reset_value();

private:
    double distance_const = 9.0;
    int current_speed;
    long long distance;
    bool encoder;
    TMC2209Stepper driver;
    timer_config_t encode_timer_config;
};

class Steer{
public:
    void steer_setup();
    void turn(int input);
    int get_current_steer() { return current_steer ; } 
    const int max_steer = 100 ; 
private:
    mcpwm_config_t servo;
    int current_steer = 0 ; 
};

#if !defined(NO_MOTOR)
extern Motor motor;
#endif

#if !defined(NO_STEER)
extern Steer steer;
#endif

#endif