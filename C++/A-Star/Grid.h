#include <iostream>
#include <cstdint>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Grid{
    public:
    //Attributes
        char value;
        uint16_t x;
        uint16_t y;
        uint16_t g_cost = 0;
        uint16_t h_cost = 0;
        uint16_t f_cost = 0;
        uint8_t state = 2;  //0:close    1:open  2:unkown
        uint16_t direction = 0;  //0: +x  1: -y  2: -x   3: +y 
        Grid *parent;
    //Methods

    //constructors
    Grid(){};

    Grid(char val, uint16_t i, uint16_t j){
        value = val;
        x = i;
        y = j;
    };
};