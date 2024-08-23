#ifndef MODULES_H
#define MODULES_H

#include "Common.h"

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_MASTER_FREQ (I2C_APB_CLK_FREQ / 200)
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_TX_BUF_DISABLE 0 
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TIMEOUT_MS 50

#define MUX_A GPIO_NUM_17
#define MUX_B GPIO_NUM_5
#define MUX_C GPIO_NUM_18
#define MUX_D GPIO_NUM_19
#define MODULES_RESET GPIO_NUM_32
#define LED_BUILTIN GPIO_NUM_2
#define REAR_LIGHT 13
#define HEAD_LIGHT 23
#define BUZZER 15
#define BATTERY_READ 34
#define BATTERY_LIGHT 12

#define VFR  0b000000000001
#define VFMR 0b000000000010
#define VF   0b000000000100
#define VFML 0b000000001000
#define VFL  0b000000010000
#define VL   0b000000100000
#define VR   0b000001000000
#define VBR  0b000010000000
#define VBL  0b000100000000
#define VBML 0b001000000000
#define VB   0b010000000000
#define VBMR 0b100000000000

#define BUFF_LEN 127
#define Compass_Address 0x60


#define MPU_Sensor_Address 0x68
#define MPU_REG_CONFIG 0x1A
#define MPU_REG_GYR_CONFIG 0x1B
#define MPU_REG_ACCEL_CONFIG 0x1C
#define MPU_PWR_MGMT_1 0x6B
#define MPU_REG_ACCEL_X_OUT_H 0x3B
#define MPU_REG_GYR_Z_H 0x47
#define MPU_REG_GYR_Z_L 0x48
#define MPU_WHO_AM_I 0x75
#define MPU_RESET_BIT 7
#define MPU_GYRO_FREQ 8000 // 8KHz

#define MPU_GYR_SCALE 32.8
#define MPU_ACCEL_SCALE 4096

#define ADC_V_MAX 12.6
#define ADC_D_MAX 4095

void i2c_bus_updater(void *);
void jetson_rx(void *);
void support_rx(void*);

class Modules
{
public:
    void module_setup();
    void reset_modules();

    void left_flasher();
    void right_flasher();
    void both_flasher();
    void lights_off();
    void headlights();
    void head_lights_on();
    void rear_lights_on();

    bool cmps_available() { return CMPS; }
    bool mpu_available() { return MPU; }
    void Buzzer(int duty)
    {
        buzzer.cmpr_a = duty;
        mcpwm_init(MCPWM_UNIT_0 , MCPWM_TIMER_1 , &buzzer);
    }

protected:
    void MUX_controller(int Select);
    bool CMPS = false;
    bool MPU = false;
    double expsmoothing(double coefficient, double new_data, double previous_data);

private:
    void i2c_bus_setup();
    mcpwm_config_t buzzer;
    bool is_remote;
};

class VL_Sensor : public Modules
{
    friend void i2c_bus_updater(void *);

public:
    void VL_setup();
    void start_vl(int selected_VL);
    void update_data(int selected_VL);
    void stop_vl(int selected_VL);
    void test_VLs();
    enum VL_name
    {
        FRONT_RIGHT,
        FRONT_MID_RIGHT,
        FRONT,
        FRONT_MID_LEFT, // Z2
        FRONT_LEFT,
        LEFT,
        RIGHT,
        BACK_RIGHT,
        BACK_LEFT, // Z1
        BACK_MID_LEFT,
        BACK,
        BACK_MID_RIGHT,
    };
    int get_data(int selected_VL) { return VL_data[selected_VL]; };
private:
    SFEVL53L1X vl;
    const int not_valid = 5000;
    volatile int VL_data[12] = {5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000};
    int chosen_VL = 0;
};

class Compass : public Modules
{
    friend void i2c_bus_updater(void *);
    friend void support_rx(void*);
private:
    i2c_cmd_handle_t cmps_cmd;
    uint8_t i2c_cmd[I2C_LINK_RECOMMENDED_SIZE(2)] = {0};
    uint8_t cmps_result_temp[2] = {0}, commands[17] = {0x02, 0x0, 0xE0, 0x0, 0xE5, 0x0, 0xE2, 0x0, 0x98, 0x0, 0x95, 0x0, 0x99, 0x0, 0x80, 0x0, 0x87};
    volatile double cmps_result[4] = {0};
    int remaining_angle = 0;
    int optimized_steer = 0;
    int previous_data = 0;
    int frozen_counter = 0; // not tested
    bool is_corrupted = false;

public:
    void cmps_setup();
    void cmps_i2c_command(uint8_t *);
    bool cmps_check();
    void cmps_erase_calibration();
    void cmps_disable_calibration();
    void cmps_enable_calibration();
    // void calculate_remaining_angle(int target_angle);
    // void calculate_optimized_steer(int remianing_angle);

    TaskHandle_t cmps_handler;

    double get_car_angle();
    void update();
    // int turn_to_angle(int target_angle); //!!! Not to be here, move to motion.cpp
    bool get_cmps_corruption_status() { return is_corrupted; }
};

class Senesor_MPU : public Modules
{
    friend void i2c_bus_updater(void *);
    friend void support_rx(void*);
private:
    i2c_cmd_handle_t mpu_cmd;
    uint8_t i2c_cmd[I2C_LINK_RECOMMENDED_SIZE(4)] = {0};
    uint8_t mpu_result_temp[14] = {0},
            commands[10] = {0x3B, 0x43, 0x6B, 0x0, 0x1A, 0x1, 0x1B, 0x2, 0x1C, 0x2};
    volatile int mpu_result[6] = {0};
    int first_previous_data = 0,
        second_previous_data = 0;

    double alpha = 0.59,
           beta = 0.22;

    double gyro_X = 0.0,
           gyro_Y = 0.0,
           gyro_Z = 0.0;
    
    double pitch = 0.0,
           roll = 0.0,
           yaw = 0.0;

    void mpu_i2c_command(uint8_t *);
    bool mpu_check();
    void mpu_clear_reg();
    void mpu_set_sensitivity();
    void mpu_register_read(uint8_t reg_addr, uint8_t *data, size_t len);
    void mpu_register_write_byte(uint8_t reg_addr, uint8_t data);

public:
    void mpu_setup();
    void mpu_register_update();
    double mpu_get_yaw();
};
class Uart
{
public:
    void config_uart();
    void debug_print(const char *input);
    void debug_print(int input);
    void debug_print(double input);
    void debug_println(const char *input);
    void debug_println(int input);
    void debug_println(double input);
    void debug_printtab(const char *input);
    void debug_printtab(int input);
    void debug_printtab(double input);
    void jetson_print(const char *input);
    void jetson_print(int input);
    void jetson_print(double input);
    void jetson_println(const char *input);
    void jetson_println(int input);
    void jetson_println(double input);
    void jetson_printtab(const char *input);
    void jetson_printtab(int input);
    void jetson_printtab(double input);
    void support_print(const char *input);
    void support_print(int input);
    void support_print(double input);
    void support_println(const char *input);
    void support_println(int input);
    void support_println(double input);
    void support_printtab(const char *input);
    void support_printtab(int input);
    void support_printtab(double input);
    void collection(const char *format, ...);
};

class Jetson
{
        friend void jetson_rx(void *);
    private:
        int angle = 0
            ,distance_from_midline = 0
            ,distance_from_sign = 0
            ,sign = 19
            ,light = 0
            ,previous_trajectory = 0
            ,flag = 0
            ,crosswalk_angle = 0
            ,crosswalk_distance = 0
            ,traffic_sign = 0
            ,traffic_distance = 0
            ,line_direction = 1
            ,side_detection = 1;
        bool passing_zone = false;
    public:
        enum SIGNS
        {
            BARRED,
            CROSSWALK,
            DOWNHILL,
            GIVEWAY,
            LEFT,
            NOPASSING,
            NOPASSINGEND,
            PARK,   //NOT USE
            RIGHT,
            STOP,
            STRAIGHT,
            TRAFFICLIGHT_GREEN,
            TRAFFICLIGHT_RED,
            TRAFFICLIGHT_YELLOW,
            TUNNEL,
            TUNNELEND,
            UPHILL,
            PARK_LEFT,  //CROSS PARK
            PARK_RIGHT, //PARALLEL PARK
            TRAVERSE,
            PARK_RIGHT_EXIT,
            TEST
        };
        enum TRAFFIC_LIGHT
        {
            NO_TRAFFIC_LIGHT,
            GREEN = 11,
            RED = 12
        };

        void jetson_uart_setup();
        int get_angle() {return angle;};
        int get_distance_from_midline() {return distance_from_midline;}
        int get_distance_from_sign() {return distance_from_sign * 10;}
        int get_sign() { return sign;};
        int get_light() {return light;};
        int get_previous_trajectory() {return previous_trajectory;};
        int get_flag() {return flag;};
        int get_crosswalk_distance(){return crosswalk_distance;};
        int get_crosswalk_angle() {return -crosswalk_angle;};
        int get_traffic_light(){return traffic_sign;};
        int get_traffic_light_distance(){return traffic_distance;};
        int get_line_direction(){return line_direction;};
        int get_side_detection(){return side_detection;};
        bool get_passing_zone() {return passing_zone;};
        void set_angle(const int&& input) {angle = input;};
        void set_distance_from_midline(const int&& input) {distance_from_midline = input;};
        void set_distance_from_sign(const int&& input) {distance_from_sign = input;};
        void set_sign(const int&& input) { sign = input ;};
        void set_light(const int&& input) { light = input;};
        void set_previous_trajectory(const int&& input) {previous_trajectory = input;};
        void set_flag(const int&& input) {flag = input;};
        void set_crosswalk_angle(const int&& input) {crosswalk_angle = input;};
        void set_crosswalk_distance(const int&& input) {crosswalk_distance = input;};
};

class Support
{
    friend void support_rx(void*);
    private:
        char data[20] = {0};
    public:
        char* get_data();
};

#if !defined(NO_MODULES)
extern Modules modules;
#endif

#if !defined(NO_VL)
extern VL_Sensor vl;
#endif

#if !defined(NO_UART)
extern Uart uart;
#endif

#if !defined(NO_COMPASS)
extern Compass cmps;
#endif

#if !defined(NO_MPU)
extern Senesor_MPU sen_MPU;
#endif

#if !defined(NO_JETSON)
extern Jetson vision;
#endif

#if !defined(NO_SUPPORT)
extern Support support;
#endif

#endif