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
#define MUX GPIO_NUM_17
#define V_MUX GPIO_NUM_13
#define MOTOR_EN_PIN GPIO_NUM_33
#define MOTOR_EN_BUTTON GPIO_NUM_15
#define BATTERY_LIGHT 12 //12.6 -> 2.27 11.4 -> 2.05
#define CONTROL HIGH
#define SUPPORT LOW
#define Compass_Address 0x60
#define BUFF_LEN 127

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



void i2c_bus_updater(void *);
static void IRAM_ATTR motor_en_isr();

class Modules
{
    friend void IRAM_ATTR motor_en_isr();
public:
    void setup();
    void set_controller(int input) { gpio_set_level(MUX, input); }

protected:
    bool CMPS = false;
    bool Mpu = false;

private:
    void i2c_bus_setup();
    
};

class Compass : public Modules
{
    friend void i2c_bus_updater(void *);

public:
    void setup();
    double get_car_angle();
    void update();

private:
    i2c_cmd_handle_t cmps_cmd;
    uint8_t i2c_cmd[I2C_LINK_RECOMMENDED_SIZE(2)] = {0};
    uint8_t cmps_result_temp[2] = {0}, commands[17] = {0x02, 0x0, 0xE0, 0x0, 0xE5, 0x0, 0xE2, 0x0, 0x98, 0x0, 0x95, 0x0, 0x99, 0x0, 0x80, 0x0, 0x87};
    volatile double cmps_result[4] = {0};
    int previous_data = 0;

    void i2c_command(uint8_t *);
    bool check();
    void erase_calibration();
    void disable_calibration();
    void enable_calibration();
};

class MPU : public Modules
{
    friend void i2c_bus_updater(void *);

public:
    void mpu_setup();
    void mpu_register_update();
    double mpu_get_yaw();

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
    void control_print(const char *input);
    void control_print(int input);
    void control_print(double input);
    void control_println(const char *input);
    void control_println(int input);
    void control_println(double input);
    void control_printtab(const char *input);
    void control_printtab(int input);
    void control_printtab(double input);
    void collection(const char *format, ...);
};

#if !defined(NO_MODULES)
extern Modules modules;
#endif

#if !defined(NO_UART)
extern Uart uart;
#endif

#if !defined(NO_COMPASS)
extern Compass cmps;
#endif

#if !defined(NO_MPU)
extern MPU mpu;
#endif

#endif