#include "A_Star.h"

#define X 18
#define Y 8

AStar astar;

void AStar::readMap(){
    for(int i = 0 ;i < X; i++){
        for(int j = 0; j < Y; j++){
            grid[i][j] = Grid(map[i][j], i, j);
        }
    }
}

void AStar::initializeStartPoint(int x_start, int y_start){ //initialize start point attributes
    grid[x_start][y_start].x = x_start;
    grid[x_start][y_start].y = y_start;
    grid[x_start][y_start].parent = &grid[x_start][y_start];
    grid[x_start][y_start].g_cost = 0;
    grid[x_start][y_start].h_cost = sqrt(pow((end_x - x_start) , 2) + pow((end_y - y_start) , 2)) * unit;
    grid[x_start][y_start].f_cost = grid[x_start][y_start].g_cost + grid[x_start][y_start].h_cost;
    grid[x_start][y_start].direction = 2;
    pathFinding_x = end_x;
    pathFinding_y = end_y;
    path.clear();
    heap.clear();
    heap.insertKey(grid[x_start][y_start]); //put start point in heap
}

int AStar::carDirection_updater(int car_direction, uint16_t past_x, uint16_t past_y, uint16_t new_x, uint16_t new_y){
    int new_carDirection = 0;
    int dif_x = new_x - past_x;
    int dif_y = new_y - past_y;
    if(car_direction == 0){
        if(dif_y == 0 && dif_x != 0){
            new_carDirection = 0;
        }else if((dif_y > 0 && dif_x > 0) || (dif_y < 0 && dif_x < 0)){
            new_carDirection = 3;
        }else if((dif_y > 0 && dif_x < 0) || (dif_y < 0 && dif_x > 0)){
            new_carDirection = 1;
        }
    }else if(car_direction == 1){
        if(dif_x == 0 && dif_y != 0){
            new_carDirection = 1;
        }else if((dif_y > 0 && dif_x > 0) || (dif_y < 0 && dif_x < 0)){
            new_carDirection = 2;
        }else if((dif_y > 0 && dif_x < 0) || (dif_y < 0 && dif_x > 0)){
            new_carDirection = 0;
        }
    }else if(car_direction == 2){
        if(dif_y == 0 && dif_x != 0){
            new_carDirection = 2;
        }else if((dif_y > 0 && dif_x > 0) || (dif_y < 0 && dif_x < 0)){
            new_carDirection = 1;
        }else if((dif_y > 0 && dif_x < 0) || (dif_y < 0 && dif_x > 0)){
            new_carDirection = 3;
        }
    }else if(car_direction == 3){
        if(dif_x == 0 && dif_y != 0){
            new_carDirection = 3;
        }else if((dif_y > 0 && dif_x > 0) || (dif_y < 0 && dif_x < 0)){
            new_carDirection = 0;
        }else if((dif_y > 0 && dif_x < 0) || (dif_y < 0 && dif_x > 0)){
            new_carDirection = 2;
        }
    }

    return new_carDirection;
}

int AStar::calculateNewCost(int i, int j){
  int g_cost = unit;  //Lateral or Longitudinal move
  if(i != 0 && j != 0){
    g_cost = g * unit * 2.8;  //Diagonal move
  }
  int h_cost = h * sqrt(pow((end_x - (new_x + i)) , 2) + pow((end_y - (new_y + j)) , 2)) * unit;
  int new_cost = grid[new_x][new_y].g_cost + g_cost + h_cost;
  return new_cost;
}

void AStar::pathFinding(){
    path.push_back(grid[pathFinding_x][pathFinding_y]);
    int x = grid[pathFinding_x][pathFinding_y].parent->x;
    int y = grid[pathFinding_x][pathFinding_y].parent->y;
    pathFinding_x = x;
    pathFinding_y = y;
    if(pathFinding_x == start_x && pathFinding_y == start_y){
        path.push_back(grid[pathFinding_x][pathFinding_y]);
        iterator = path.size() - 1;
        return;
    }
    vTaskDelay(10);
    
    pathFinding();
}

void AStar::HybridAstar(){
  Grid current_node = heap.getMin();

  grid[current_node.x][current_node.y].state = 0;
  heap.extractMin();

  if(current_node.x == end_x && current_node.y == end_y){
    grid[end_x][end_y].parent = current_node.parent;
    return;
  }

  new_x = current_node.x;
  new_y = current_node.y;

if(grid[new_x][new_y].direction == 0){  //+x
  for(int i = -1; i < 2; i += 2){
    for(int j = -1; j < 2; j++){
      if((new_x + i) < 0 || (new_x + i) >= X || (new_y + j) < 0 || (new_y + j) >= Y || ((i == 0) && (j == 0))){
        continue;
      }
      if(grid[new_x + i][new_y + j].value == 'h' || grid[new_x + i][new_y + j].state == 0){
        continue;
      }

      //update neighbour's costs
      int new_f_cost = calculateNewCost(i , j);
      if(new_f_cost < grid[new_x + i][new_y + j].f_cost || grid[new_x + i][new_y + j].state != 1){
        int new_g_cost = unit;
        if(i != 0 && j != 0){
          new_g_cost = unit * 2.8;
        }
        grid[new_x + i][new_y + j].g_cost = new_g_cost;
        grid[new_x + i][new_y + j].h_cost = sqrt(pow((end_x - (new_x + i)) , 2) + pow((end_y - (new_y + j)) , 2)) * unit;
        grid[new_x + i][new_y + j].f_cost = new_f_cost; 

        grid[new_x + i][new_y + j].parent = &grid[new_x][new_y];  //update neighbour's parent
        grid[new_x + i][new_y + j].direction = carDirection_updater(grid[new_x + i][new_y + j].parent->direction, grid[new_x + i][new_y + j].parent->x, grid[new_x + i][new_y + j].parent->y, grid[new_x + i][new_y + j].x, grid[new_x + i][new_y + j].y);    //update neighbour's direction

        if(grid[new_x + i][new_y + j].state != 1){
          heap.insertKey(grid[new_x + i][new_y + j]);
          grid[new_x + i][new_y + j].state = 1; //add this neighbour to open list
        }
      }
    }
  }
}else if(grid[new_x][new_y].direction == 1){  //-y
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j += 2){
            if((new_x + i) < 0 || (new_x + i) >= X || (new_y + j) < 0 || (new_y + j) >= Y || ((i == 0) && (j == 0))){
                continue;
            }
            if(grid[new_x + i][new_y + j].value == 'h' || grid[new_x + i][new_y + j].state == 0){
                continue;
            }
            
              //update neighbour's costs
              int new_f_cost = calculateNewCost(i , j);
              if(new_f_cost < grid[new_x + i][new_y + j].f_cost || grid[new_x + i][new_y + j].state != 1){
                int new_g_cost = unit;
                if(i != 0 && j != 0){
                  new_g_cost = unit * 2.8;
                }
                grid[new_x + i][new_y + j].g_cost = new_g_cost;
                grid[new_x + i][new_y + j].h_cost = sqrt(pow((end_x - (new_x + i)) , 2) + pow((end_y - (new_y + j)) , 2)) * unit;
                grid[new_x + i][new_y + j].f_cost = new_f_cost;

                grid[new_x + i][new_y + j].parent = &grid[new_x][new_y];  //update neighbour's parent
                grid[new_x + i][new_y + j].direction = carDirection_updater(grid[new_x + i][new_y + j].parent->direction, grid[new_x + i][new_y + j].parent->x, grid[new_x + i][new_y + j].parent->y, grid[new_x + i][new_y + j].x, grid[new_x + i][new_y + j].y);    //update neighbour's direction

                if(grid[new_x + i][new_y + j].state != 1){
                heap.insertKey(grid[new_x + i][new_y + j]);
                grid[new_x + i][new_y + j].state = 1; //add this neighbour to open list
                }
            }
        }
    }
}else if(grid[new_x][new_y].direction == 2){  //-x
    for(int i = -1; i < 2; i += 2){
        for(int j = -1; j < 2; j++){
            if((new_x + i) < 0 || (new_x + i) >= X || (new_y + j) < 0 || (new_y + j) >= Y || ((i == 0) && (j == 0))){
                continue;
            }
            if(grid[new_x + i][new_y + j].value == 'h' || grid[new_x + i][new_y + j].state == 0){
                continue;
            }
            
            int new_cost = calculateNewCost(i , j);
            if(new_cost < grid[new_x + i][new_y + j].f_cost || grid[new_x + i][new_y + j].state != 1){
                grid[new_x + i][new_y + j].f_cost = new_cost; //update neighbour's cost
                grid[new_x + i][new_y + j].parent = &grid[new_x][new_y];  //update neighbour's parent
                grid[new_x + i][new_y + j].direction = carDirection_updater(grid[new_x + i][new_y + j].parent->direction, grid[new_x + i][new_y + j].parent->x, grid[new_x + i][new_y + j].parent->y, grid[new_x + i][new_y + j].x, grid[new_x + i][new_y + j].y);    //update neighbour's direction

                if(grid[new_x + i][new_y + j].state != 1){
                heap.insertKey(grid[new_x + i][new_y + j]);
                grid[new_x + i][new_y + j].state = 1; //add this neighbour to open list
                }
            }
        }
    }    
}else if(grid[new_x][new_y].direction == 3){  //+y
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j += 2){
            if((new_x + i) < 0 || (new_x + i) >= X || (new_y + j) < 0 || (new_y + j) >= Y || ((i == 0) && (j == 0))){
                continue;
            }
            if(grid[new_x + i][new_y + j].value == 'h' || grid[new_x + i][new_y + j].state == 0){
                continue;
            }
            
              //update neighbour's costs
              int new_f_cost = calculateNewCost(i , j);
              if(new_f_cost < grid[new_x + i][new_y + j].f_cost || grid[new_x + i][new_y + j].state != 1){
                int new_g_cost = unit;
                if(i != 0 && j != 0){
                  new_g_cost = unit * 2.8;
                }
                grid[new_x + i][new_y + j].g_cost = new_g_cost;
                grid[new_x + i][new_y + j].h_cost = sqrt(pow((end_x - (new_x + i)) , 2) + pow((end_y - (new_y + j)) , 2)) * unit;
                grid[new_x + i][new_y + j].f_cost = new_f_cost;

                grid[new_x + i][new_y + j].parent = &grid[new_x][new_y];  //update neighbour's parent
                grid[new_x + i][new_y + j].direction = carDirection_updater(grid[new_x + i][new_y + j].parent->direction, grid[new_x + i][new_y + j].parent->x, grid[new_x + i][new_y + j].parent->y, grid[new_x + i][new_y + j].x, grid[new_x + i][new_y + j].y);    //update neighbour's direction

                if(grid[new_x + i][new_y + j].state != 1){
                heap.insertKey(grid[new_x + i][new_y + j]);
                grid[new_x + i][new_y + j].state = 1; //add this neighbour to open list
                }
            }
        }
    } 
}
    // uart.debug_print("helo");
  HybridAstar();  //Astar loop
}

AStar::AStar() : current_instruction(&AStar::selector) 
{}

void AStar::instruction(){
    (this->*current_instruction)();
    // uart.debug_print("he");
    vTaskDelay(2); 
}

void AStar::selector(){
    // uart.debug_println("1");
    int delta_x, delta_y;
    if(iterator == 0){
        motion.stop();
        uart.debug_println("kos nane");
        // vTaskDelay(30000);
    }
    else{
        // delta_x = 10;
        // delta_y = 10;
        // direction = 0;
        
        delta_x = path.at(iterator - 1).x - path.at(iterator).x;
        delta_y = path.at(iterator - 1).y - path.at(iterator).y;

        direction = path.at(iterator).direction;
        vTaskDelay(500);
        iterator -= 1;
    }

    if(direction == 2){
        if(delta_y == 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead;
        }
        else if(delta_y == 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back;
        }
        else if(delta_y > 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead_left;
        }
        else if(delta_y < 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back_right;
        }
        else if(delta_y > 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back_left;
        }
        else if(delta_y < 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead_right;
        }
    }
    else if(direction == 0){
        if(delta_y == 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back;
        }
        else if(delta_y == 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead;
        }
        else if(delta_y > 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back_right;
        }
        else if(delta_y < 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead_left;
        }
        else if(delta_y > 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead_right;
        }
        else if(delta_y < 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back_left;
        }
    }
    else if(direction == 1){
        if(delta_y < 0 && delta_x == 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back;
        }
        else if(delta_y > 0 && delta_x == 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead;
        }
        else if(delta_y > 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead_right;
        }
        else if(delta_y < 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back_left;
        }
        else if(delta_y > 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead_left;
        }
        else if(delta_y < 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back_right;
        }
    }
    else if(direction == 3){
        if(delta_y < 0 && delta_x == 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead;
        }
        else if(delta_y > 0 && delta_x == 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back;
        }
        else if(delta_y > 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back_left;
        }
        else if(delta_y < 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead_right;
        }
        else if(delta_y > 0 && delta_x < 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_back_right;
        }
        else if(delta_y < 0 && delta_x > 0){
            motor.start_encode();
            vTaskDelay(100);
            current_instruction = &AStar::go_ahead_left;
        }
    }
}

void AStar::go_ahead(){
    if(motor.get_distance_covered_mm() < 500){
        motion.operate(motion.go_straight(), 30);
    }
    else{
        motion.stop();
        motor.start_encode();
        vTaskDelay(100);
        current_instruction = &AStar::selector;
    }
}

void AStar::go_back(){
    if(motor.get_distance_covered_mm() < 500){
        motion.operate(motion.go_straight(), -30);
    }
    else{
        motion.stop();
        motor.start_encode();
        vTaskDelay(100);
        current_instruction = &AStar::selector;
    }
}

void AStar::go_ahead_right(){
    if(motion.go_for_angle(90, -90, 30)){
        motion.stop();
        motor.start_encode();
        vTaskDelay(100);
        current_instruction = &AStar::selector;
    }
}

void AStar::go_ahead_left(){
    if(motion.go_for_angle(90, 90, 30)){
        motion.stop();
        motor.start_encode();
        vTaskDelay(100);
        current_instruction = &AStar::selector;
    }
}

void AStar::go_back_right(){
    if(motion.go_for_angle(90, -90, -30)){
        motion.stop();
        motor.start_encode();
        vTaskDelay(100);
        current_instruction = &AStar::selector;
    }
}

void AStar::go_back_left(){
    if(motion.go_for_angle(90, 90, -30)){
        motion.stop();
        motor.start_encode();
        vTaskDelay(100);
        current_instruction = &AStar::selector;
    }
}
