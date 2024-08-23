#include <iostream>
#include <fstream>
#include <sstream>
#include "MinHeap.h"

using namespace std;

#define X 18
#define Y 8

MinHeap heap; //create heap
Grid grid[X][Y];  //map size

//variables
double g = 1;
double h = 1;
uint16_t unit = 50;
uint16_t start_x = 0;
uint16_t start_y = 0;
uint16_t end_x = 7;
uint16_t end_y = 7;

uint16_t new_x = 0;
uint16_t new_y = 0;

vector<Grid *> openList;

//file functions
void readMap(string mapName){ //reading the map from the text file
    ifstream file(mapName);

    if(!file.is_open()){
      cout << "Unable to open file" << endl;
      return ;
    }

    vector<char> data;
    string line;
    uint16_t i = 0;
    uint16_t j = 0;

    while(getline(file, line)){
      for(auto c : line){
        grid[i][j] = Grid(c, i, j);
        j++;
      }
      i++;
      j = 0;
    }
    cout << "read map" << endl;
    file.close();
}

void copyFile(const string& inputFileName, const string& outputFileName){
  ifstream inputFile(inputFileName);
  ofstream outputFile(outputFileName);

  if(inputFile.is_open() && outputFile.is_open()){
    string line;

    //Read the content from the input file line by line and write it to the output file
    while(getline(inputFile, line)){
      outputFile << line << endl;
    }

    // Close the files
    inputFile.close();
    outputFile.close();
    cout << "copied" << endl;
  }else{
    cout << "Unable to open files." << endl;
  }
}

void rewriteStar(const string& filename, int row, int column, char newChar){
    ifstream fileIn(filename);
    string line;
    string fileContents;

    // Read the file and store its contents
    while(getline(fileIn, line)){
        fileContents += line + "\n";
    }
    fileIn.close();

    // Modify the character at the given row and column
    int index = 0;
    int currentRow = 0;
    while(index < fileContents.length()){
        if(currentRow == row){
            fileContents[index + column - 1] = newChar;
            break;
        }
        if(fileContents[index] == '\n'){
            currentRow++;
        }
        index++;
    }

    // Rewrite the modified contents back to the file
    ofstream fileOut(filename);
    fileOut << fileContents;
    fileOut.close();
}

//algorithm functions
void initializeStartPoint(int x_start, int y_start){  //initialize start point attributes
  grid[x_start][y_start].x = x_start;
  grid[x_start][y_start].y = y_start;
  grid[x_start][y_start].parent = &grid[x_start][y_start];
  grid[x_start][y_start].g_cost = 0;
  grid[x_start][y_start].h_cost = sqrt(pow((end_x - x_start) , 2) + pow((end_y - y_start) , 2)) * unit;
  grid[x_start][y_start].f_cost = grid[x_start][y_start].g_cost + grid[x_start][y_start].h_cost;
  grid[x_start][y_start].direction = 2;

  heap.insertKey(grid[x_start][y_start]); //put start point in heap
}

int carDirection_updater(int car_direction, uint16_t past_x, uint16_t past_y, uint16_t new_x, uint16_t new_y){
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

int calculateNewCost(int i, int j){
  int g_cost = unit;  //Lateral or Longitudinal move
  if(i != 0 && j != 0){
    g_cost = g * unit * 2.8;  //Diagonal move
  }
  int h_cost = h * sqrt(pow((end_x - (new_x + i)) , 2) + pow((end_y - (new_y + j)) , 2)) * unit;
  int new_cost = grid[new_x][new_y].g_cost + g_cost + h_cost;
  return new_cost;
}

void pathFinding(){
  rewriteStar("Hdest.txt", end_x, end_y, '*');
  grid[end_x][end_y].value = '*';
  int x = grid[end_x][end_y].parent->x;
  int y = grid[end_x][end_y].parent->y;
  end_x = x;
  end_y = y;
  if(end_x == start_x && end_y == start_y){
    return;
  }
  pathFinding();
}

void Astar(){
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

  Astar();  //Astar loop
}

int main(){
  readMap("Hmap.txt");  //fill the grids with map data
  initializeStartPoint(start_x, start_y); //initialize start point attributes and add it to openList
  Astar();

  copyFile("Hmap.txt", "Hdest.txt");
  pathFinding();
  for(int i = 0; i < X; i++){
    for(int j = 0; j < Y; j++){
      if(grid[i][j].value == '*'){
        cout << "x : " << grid[i][j].x << " y : " << grid[i][j].y << " dir : " << grid[i][j].direction << endl;
      }
    }
  }

  cout << grid[0][1].f_cost << endl;
  cout << grid[1][1].f_cost << endl;

  return 0;
}

