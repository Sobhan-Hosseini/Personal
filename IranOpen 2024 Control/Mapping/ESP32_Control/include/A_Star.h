#ifndef A_STAR
#define A_STAR

#include "Motion.h"
#include "MinHeap.h"
#include "Mapping.h"

#define X 18
#define Y 8


class AStar {
    public:
        MinHeap heap; //create heap
        Grid grid[X][Y];  //map size

        //Map
        char map[X][Y] = 
        {
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'},
         {'0', '0', '0', '0', '0', '0', '0', '0'}};

        //variables
        double g = 1;
        double h = 1;
        uint16_t unit = 50;
        uint16_t start_x = 0;
        uint16_t start_y = 0;
        uint16_t end_x = 1;
        uint16_t end_y = 1;
        uint16_t pathFinding_x = 17;
        uint16_t pathFinding_y = 7;
        uint16_t new_x = 0;
        uint16_t new_y = 0;

        vector<Grid *> openList;
        vector<Grid> path;

        int direction;
        int iterator = 0;

        void initializeStartPoint(int x_start, int y_start);
        void pathFinding();
        void HybridAstar();
        void readMap();
        int carDirection_updater(int car_direction, uint16_t past_x, uint16_t past_y, uint16_t new_x, uint16_t new_y);
        int calculateNewCost(int i, int j);

        void(AStar::*current_instruction)();
        void selector();
        void go_ahead();
        void go_ahead_right();
        void go_ahead_left();
        void go_back();
        void go_back_right();
        void go_back_left();
    
        void instruction();
        AStar();

};

#if !defined(NO_GLOBAL_MAPPING)
extern AStar astar;
#endif
#endif