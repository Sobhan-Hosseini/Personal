/*
  Maze Traversal
  Author: Seyed Sobhan Hosseini
  Date: Summer 2023
*/

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

bool isSafe(char map[12][12], int x, int y){
	if(x >= 0 && x < 12 && y >= 0 && y < 12 && map[x][y] == '.')
		return true;
	return false;
}

bool solveMaze(char map[12][12], int x, int y, int outr, int outc, char sol[12][12])
{
	bool flag = false ; 
	if(x == outr && y == outc && map[x][y] == '.'){
		sol[x][y] = '.';
		return true;
	}
	if(isSafe(map, x, y) == true){
		if(sol[x][y] == '.')
			return false;
		sol[x][y] = '.';
		
		if(solveMaze(map, x - 1, y, outr, outc, sol) == true)
			flag = true ; 

		if(solveMaze(map, x + 1, y, outr, outc, sol) == true)
			flag = true ; 

		if (solveMaze(map, x, y - 1, outr, outc, sol) == true)
			flag = true ; 

		if (solveMaze(map, x, y + 1, outr, outc, sol) == true)
			flag = true ; 
	
	if(flag){
		return true;
	}
		sol[x][y] = 'x';
		return false;
	}
	return false;
}

void printSolution(char sol[12][12]){
	for(int i = 0; i < 12; i++){
		for(int j = 0; j < 12; j++)
			cout << sol[i][j] << " ";
			cout << endl;
	}
}

bool solve(char map[12][12], int inr, int inc, int outr, int outc){
	char sol[12][12] = {
	    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    };
	if(solveMaze(map, inr, inc, outr, outc, sol) == false){
		cout<<"Solution doesn't exist";
		return false;
	}
	printSolution(sol);
	return true;
}

void MazeTraversal(char map[12][12], int inr, int inc){
	int outr, outc;
    for(int i = 0; i < 12; i++){	
    	if(map[11][i] == '.'){
    		outr = 11;
    		outc = i; 
		}else if(map[0][i] == '.'){
			outr = 0;
    		outc = i;
		}else if(map[i][11] == '.'){
			outr = i;
    		outc = 11;
		}else if(map[i][0] == '.'){
			outr = i;
    		outc = 0;
		} 
	}
	solve(map, inr, inc, outr, outc);
}

int main(){
	int inr, inc;
	char map[12][12] {
	    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#'},
		{'.', '.', '#', '.', '#', '.', '#', '#', '#', '#', '.', '#'},
		{'#', '#', '#', '.', '#', '.', '.', '.', '.', '#', '.', '#'},
		{'#', '.', '.', '.', '.', '#', '#', '#', '.', '#', '.', '.'},
		{'#', '#', '#', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
		{'#', '.', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
		{'#', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
		{'#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
		{'#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '.', '#'},
		{'#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    };
    
    cout << "Enter your start point : " << endl;
    cout << "Row : ";
    cin >> inr;
    inr = inr - 1;
    cout << "Column : ";
    cin >> inc;
    inc = inc - 1;
    cout << endl;
    cout << "Your Map :" << endl;
    for(int i = 0; i < 12; i++){
    	for(int j = 0; j < 12; j++){
    		cout << map[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "The Solution : " << endl;
    MazeTraversal(map, inr, inc);
	
	return 0;
}


