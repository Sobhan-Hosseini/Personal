#ifndef COMMON_H
#define COMMON_H

#ifndef CITY
    #define CITY 0
#endif
#ifndef RACE
    #define RACE 1
#endif
#ifndef MODE
    #define MODE CITY
#endif

#include "Arduino.h"
#include "SparkFun_VL53L1X.h"
#include "TMCStepper.h"
#include "Adafruit_NeoPixel.h"
#include "stdarg.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "driver/timer.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#endif