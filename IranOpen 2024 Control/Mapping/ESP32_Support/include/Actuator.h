#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Common.h"

#define SERVO_OUT 16
#define SERVO_MAX_RIGHT 100
#define SERVO_MAX_LEFT -100
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define TIMER_SCALE (APB_CLK_FREQ / 128000)

class Motor
{
public:
    Motor();
    void setup();
    void UART_accelerate(int speed);

private:
    int current_speed;
    long long distance;
    TMC2209Stepper driver;
};

class Steer
{
public:
    void setup();
    void turn(int input);
    const int max_steer = 100;

private:
    mcpwm_config_t servo;
    int current_steer = 0;
};

#if !defined(NO_MOTOR)
extern Motor motor;
#endif

#if !defined(NO_STEER)
extern Steer steer;
#endif

#endif