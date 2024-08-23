/*
  A-Star Algorithm
  Authors: Amirhossein Gholizadeh - Seyed Sobhan Hosseini
  Date: ‎Spring 2024
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include "MinHeap.h"

using namespace std;

MinHeap heap; //create heap
Grid grid[240][240];  //map size

//variables
uint16_t g = 1;
uint16_t h = 1;
uint16_t unit = 10;
uint16_t start_x = 0;
uint16_t start_y = 0;
int end_x = 239;
int end_y = 239;

uint16_t new_x = 0;
uint16_t new_y = 0;

vector<Grid *> openList;
// vector<Grid *>::iterator it;

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
    cout << "File copied successfully." << endl;
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
    int currentRow = 1;
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

    cout << "Character at row " << row << ", column " << column << " has been rewritten to " << newChar << " in the file." << std::endl;
}

//algorithm functions
void initializeStartPoint(int x_start, int y_start){  //initialize start point attributes
  grid[x_start][y_start].x = x_start;
  grid[x_start][y_start].y = y_start;
  grid[x_start][y_start].parent = &grid[x_start][y_start];
  grid[x_start][y_start].g_cost = 0;
  grid[x_start][y_start].h_cost = sqrt(pow((end_x - x_start) , 2) + pow((end_y - y_start) , 2)) * unit;
  grid[x_start][y_start].f_cost = grid[x_start][y_start].g_cost + grid[x_start][y_start].h_cost;

  heap.insertKey(grid[x_start][y_start]); //put start point in heap
}

int calculateNewCost(int i, int j){
  int g_cost = unit;  //Lateral or Longitudinal move
  if(i != 0 && j != 0){
    g_cost = unit * 1.4;  //Diagonal move
  }
  int h_cost = sqrt(pow((end_x - (new_x + i)) , 2) + pow((end_y - (new_y + j)) , 2)) * unit;
  int new_cost = grid[new_x][new_y].g_cost + g_cost + h_cost;
  return new_cost;
}

void pathFinding(){
  cout<<"x: "<< end_x << " y: "<< end_y << endl;
  rewriteStar("hdest.txt", end_x, end_y, '*');
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
  cout <<"current node f cost: ";
  cout << current_node.f_cost << endl;

  grid[current_node.x][current_node.y].state = 0;
  heap.extractMin();

  if(current_node.x == end_x && current_node.y == end_y){
    grid[end_x][end_y].parent = current_node.parent;
    cout << "I'm Done!" << endl;
    return;
  }
  
  new_x = current_node.x;
  new_y = current_node.y;

  cout <<"new x: ";
  cout << new_x << endl;
  cout <<"new y: ";
  cout << new_y << endl;

  for(int i = -1; i < 2; i++){
    for(int j = -1; j < 2; j++){
      if((new_x + i) < 0 || (new_x + i) >= 240 || (new_y + j) < 0 || (new_y + j) >= 240 || ((i == 0) && (j == 0))){
        continue;
      }
      if(grid[new_x + i][new_y + j].value == 'h' || grid[new_x + i][new_y + j].state == 0){
        continue;
      }
      
      int new_cost = calculateNewCost(i , j);
      if(new_cost < grid[new_x + i][new_y + j].f_cost || grid[new_x + i][new_y + j].state != 1){
        grid[new_x + i][new_y + j].f_cost = new_cost; //update neighbour's cost
        grid[new_x + i][new_y + j].parent = &grid[new_x][new_y];  //update neighbour's parent

        cout<<"i: "<< i<<" j: "<< j <<" new cost: " << new_cost <<endl;

        if(grid[new_x + i][new_y + j].state != 1){
          heap.insertKey(grid[new_x + i][new_y + j]);
          grid[new_x + i][new_y + j].state = 1; //add this neighbour to open list
        }
      }
    }
  }

  cout<<endl;
  // grid[new_x][new_y].state = 0;
  // if(heap.hvec.size() == 0){
  //   cout << "Heap is empty!" << endl;
  // }else{
    Astar();  //Astar loop
  // }
}

int main(){
  readMap("atWork.txt");  //fill the grids with map data
  initializeStartPoint(start_x, start_y); //initialize start point attributes and add it to openList
  Astar();

  // cout<<"x_p: "<<grid[5][5].parent->x << " y_p: "<<grid[5][5].parent->y<<endl;
  // cout<<"x_p: "<<grid[4][4].parent->x << " y_p: "<<grid[4][4].parent->y<<endl;
  // cout<<"x_p: "<<grid[3][4].parent->x << " y_p: "<<grid[3][4].parent->y<<endl;
  // cout<<"x_p: "<<grid[2][3].parent->x << " y_p: "<<grid[2][3].parent->y<<endl;
  // cout<<"x_p: "<<grid[1][2].parent->x << " y_p: "<<grid[1][2].parent->y<<endl;
  // cout<<"x_p: "<<grid[0][1].parent->x << " y_p: "<<grid[0][1].parent->y<<endl;
  // cout<<"x_p: "<<grid[0][0].parent->x << " y_p: "<<grid[0][0].parent->y<<endl;

  cout<<"path: "<<endl;
  copyFile("atWork.txt", "hdest.txt");
  pathFinding();
  // pathFinding();
  // pathFinding()
  // pathFinding();

  // cout<<"x_p: "<<grid[5][5].parent->x << " y_p: "<<grid[5][5].parent->y<<endl;
  // cout<<"x_p: "<<grid[4][4].parent->x << " y_p: "<<grid[4][4].parent->y<<endl;
  // cout<<"x_p: "<<grid[3][4].parent->x << " y_p: "<<grid[3][4].parent->y<<endl;
  // cout<<"x_p: "<<grid[2][3].parent->x << " y_p: "<<grid[2][3].parent->y<<endl;
  // cout<<"x_p: "<<grid[1][2].parent->x << " y_p: "<<grid[1][2].parent->y<<endl;
  // cout<<"x_p: "<<grid[0][1].parent->x << " y_p: "<<grid[0][1].parent->y<<endl;
  // cout<<"x_p: "<<grid[0][0].parent->x << " y_p: "<<grid[0][0].parent->y<<endl;


  // A_star(start_x, start_y);
  return 0;
}

