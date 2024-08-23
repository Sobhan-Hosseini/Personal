#include "Actuator.h"
#include "Modules.h"
// #include "Motion.h" 

Motor motor;
Steer steer;
// Motion motion;
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

void IRAM_ATTR distance_timer_isr(void *)
{
    motor.distance += motor.current_speed > 0 ? motor.current_speed : -motor.current_speed;
    timer_init(TIMER_GROUP_0, TIMER_0, &motor.encode_timer_config);
}

Motor::Motor() : driver(NULL, 0.11f, DRIVER_ADDRESS)
{
    current_speed = distance = 0;
    encoder = false;
}

void Motor::motor_setup()
{
    encode_timer_config.divider = 128;
    encode_timer_config.counter_dir = TIMER_COUNT_UP;
    encode_timer_config.counter_en = TIMER_START;
    encode_timer_config.alarm_en = TIMER_ALARM_EN;
    encode_timer_config.auto_reload = TIMER_AUTORELOAD_EN;
    timer_init(TIMER_GROUP_0, TIMER_0, &encode_timer_config);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 20 * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_pause(TIMER_GROUP_0, TIMER_0);
    timer_isr_register(TIMER_GROUP_0, TIMER_0, distance_timer_isr, nullptr, ESP_INTR_FLAG_IRAM, nullptr);

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

    #if MODE == RACE
        driver.microsteps(8);
    #else
        driver.microsteps(64);
    #endif
}

void Motor::reset_value(){
    motion.reset_kappa_parameter();
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
        if (current_speed == 0)
        {
            driver.freewheel(0);
        }
        while(speed - current_speed > SPEEDSTEP)
        {
            current_speed += SPEEDSTEP;
            if(current_speed == 0)
            {
                driver.freewheel(0);
            }
            if(current_speed == 60)
            {
                vTaskDelay(50);
            }
            #if MODE == RACE
                 driver.VACTUAL(map(current_speed, -100, 100, (-500000 / 32),  (500000 / 32)));
            #else
                driver.VACTUAL(map(current_speed, -100, 100, -100000, 100000));
            #endif 
            if ((current_speed > 20) || (current_speed < -20))
            {
                driver.en_spreadCycle(true);
            }
            else
            {
                driver.en_spreadCycle(false);
            }
            // uart.debug_print("current speed: ");
            // uart.debug_println(current_speed);
            delay(2);
        }
        while (speed - current_speed < -SPEEDSTEP)
        {
            current_speed -= SPEEDSTEP;
            if(current_speed == 0)
            {
                driver.freewheel(0);
            }
            if(current_speed == -60)
            {
                vTaskDelay(50);
            }
            #if MODE == RACE
                driver.VACTUAL(map(current_speed, -100, 100, (-500000 / 32),  (500000 / 32)));
            #else
                driver.VACTUAL(map(current_speed, -100, 100, -100000, 100000));
            #endif 
            if ((current_speed > 20) || (current_speed < -20))
            {
                driver.en_spreadCycle(true);
            }
            else
            {
                driver.en_spreadCycle(false);
            }
        }
        if (speed == 0)
        {
            driver.freewheel(1);
        }
        if(speed > 20 || speed < -20)
        {
            driver.en_spreadCycle(true);
        }
        else
        {
            driver.en_spreadCycle(false);
        }
        #if MODE == RACE
            driver.VACTUAL(map(speed, -100, 100, (-500000 / 32),  (500000 / 32)));
        #else
            driver.VACTUAL(map(speed, -100, 100, -100000, 100000));
        #endif                
        current_speed = speed;
    }
}

void Steer::steer_setup()
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
    servo.cmpr_a = map(-steer, -100, +100, 3.3, 7.56); // right + ... 3.15,7.71
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &servo);
}