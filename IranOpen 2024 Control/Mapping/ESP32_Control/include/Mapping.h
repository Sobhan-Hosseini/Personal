#ifndef _MAPPING_H
#define _MAPPING_H

#include "Modules.h"
#include "Actuator.h"
#include "Motion.h"
#include <WiFi.h>
#include <WebServer.h>

class Mapping {
    public:
    const int x_resolution = 18;
    const int y_resolution = 8;
    uint16_t grid[18][8] {0};
    int x_real = 0;
    int y_real = 0;
    int x_grid, y_grid;
    int sensitivity = 500;

    void mapping();
    void begin_server();
    void client();
};

#if !defined(NO_GLOBAL_MAPPING)
extern Mapping mapping;
#endif
#endif