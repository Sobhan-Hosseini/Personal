#include "Actuator.h"
#include "Modules.h"

Motor motor;
Steer steer;

inline double map(int x, int in_min, int in_max, double out_min, double out_max)
{
    const double dividend = out_max - out_min;
    const int divisor = in_max - in_min;
    const int delta = x - in_min;
    return (double)(delta * dividend) / divisor + out_min;
}

inline int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    const int dividend = out_max - out_min;
    const int divisor = in_max - in_min;
    const int delta = x - in_min;
    return (double)(delta * dividend) / divisor + out_min;
}

Motor::Motor() : driver(NULL , 0.11f, DRIVER_ADDRESS)
{
    current_speed = 0;
    distance = 0;
}
void Motor::setup()
{

    driver.begin();
    driver.en_spreadCycle(false);
    driver.toff(4);
    driver.blank_time(10);
    driver.TCOOLTHRS(0xFFFFF); // 20bit max
    driver.semin(5);
    driver.semax(2);
    driver.shaft(true);
    driver.SGTHRS(2);
    driver.irun(17);
    driver.ihold(0);
    driver.vsense(false);
    driver.iholddelay(2);
    driver.TPOWERDOWN(12);
    driver.VACTUAL(0);
    driver.freewheel(1);
    driver.microsteps(64);
}

void Motor::UART_accelerate(int speed)
{
    if (speed < -100)
    {
        speed = -100;
    }
    else if (speed > 100)
    {
        speed = 100;
    }
    if (speed != current_speed)
    {
        if(current_speed == 0)
            driver.freewheel(0);
        while(speed - 10> current_speed){    
            current_speed += 10;
            if(current_speed == 60)
                vTaskDelay(50);
            driver.VACTUAL(map(current_speed, -100, 100, -100000, 100000));
            if (current_speed > 20 || current_speed < -20)
                driver.en_spreadCycle(true);
            else
                driver.en_spreadCycle(false);
        }
        while(speed + 10 < current_speed){
            current_speed -= 10;
            if(current_speed == -60)
                vTaskDelay(50);
            driver.VACTUAL(map(current_speed, -100, 100, -100000, 100000));
            if (current_speed > 20 || current_speed < -20)
                driver.en_spreadCycle(true);
            else
                driver.en_spreadCycle(false);           
        }
        if(speed == 0)
            driver.freewheel(1);
        if (speed > 20 || speed < -20)
            driver.en_spreadCycle(true);
        else
            driver.en_spreadCycle(false);
        driver.VACTUAL(map(speed, -100, 100, -100000, 100000));              
        current_speed = speed;
    }
}

void Steer::setup()
{
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_OUT);
    servo.frequency = 50;
    servo.cmpr_a = 5.43;
    servo.duty_mode = MCPWM_DUTY_MODE_0;
    servo.counter_mode = MCPWM_UP_COUNTER;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &servo);
    uart.debug_println("\n4.Servo motor setup done.");
}

void Steer::turn(int steer)
{
    if (steer > SERVO_MAX_RIGHT)
    {
        steer = SERVO_MAX_RIGHT;
    }
    else if (steer < SERVO_MAX_LEFT)
    {
        steer = SERVO_MAX_LEFT;
    }
    current_steer = steer;
    servo.cmpr_a = map(-steer, -100, +100, 3.15, 7.71); // rast mosbat    5.55          3.25 va 7.56
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &servo);
}