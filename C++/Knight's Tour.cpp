/*
  Knight's Tour
  Author: Seyed Sobhan Hosseini
  Date: Tuesday, May 30, 2023, 8:55:44 PM
*/

#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <unistd.h>
using namespace std;

int counter_x[8] = {1,1,2,2,-1,-1,-2,-2};
int counter_y[8] = {2,-2,1,-1,2,-2,1,-1};

bool limits(int x, int y){
    return ((x >= 0 && y >= 0) && (x < 8 && y < 8));
}

bool isempty(int a[8][8], int x, int y){
    return (limits(x, y)) && (a[x][y] < 0);
}

int degree(int a[8][8], int x, int y){
    int counter = 0;
    for (int i = 0; i < 8; i++)
        if (isempty(a, (x + counter_x[i]), (y + counter_y[i])))
            counter++;
    return counter;
}

bool nextMove(int a[8][8], int *x, int *y){
    int min = -1, z, deg = 8, next_x, next_y;
    int start = rand()%8;
    for (int count = 0; count < 8; count++){
        int i = (start + count)%8;
        next_x = *x + counter_x[i];
        next_y = *y + counter_y[i];
        if ((isempty(a, next_x, next_y)) &&(z = degree(a, next_x, next_y)) <= deg){
            min = i;
            deg = z;
        }
    }

    if(deg == -1)
        return false;
 
    next_x = *x + counter_x[min];
    next_y = *y + counter_y[min];

    a[next_x][next_y] = a[*x][*y]+1;
 
    *x = next_x;
    *y = next_y;
 
    return true;
}

void print(int a[8][8]){
	int q = 0;
	int b[8][8] = {0};
	while(q < 64){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
        	if (a[i][j] == q + 1){
        		b[i][j] = q + 1;
        	}
        }
    }
    for (int r = 0; r < 8; r++){
        for (int c = 0; c < 8; c++){
            printf("%d\t",b[r][c]);
		    }
		    printf("\n");
		cout << endl;
		cout << endl;
	    }
	sleep(1);
	system("CLS");
	q = q + 1;
    }
    if(q == 64){
    	for (int r = 0; r < 8; r++){
        for (int c = 0; c < 8; c++){
            printf("%d\t",b[r][c]);
		    }
		    printf("\n");
		cout << endl;
		cout << endl;
	    }	
	}
}

bool neighbour(int x, int y, int X, int Y){
    for (int i = 0; i < 8; i++)
        if (((x+counter_x[i]) == X)&&((y + counter_y[i]) == Y))
            return true;
 
    return false;
}

bool Tour(int m, int n){
    int a[8][8];
    for(int i = 0; i < 8; i++){
    	for(int j = 0; j < 8; j++){
    		a[i][j] = -1;
		}
	}
 
    int start_X[2] = {0, 7};
    int start_Y[2] = {0, 7};
    int start_x = start_X[m];
    int start_y = start_Y[n];

    int x = start_x, y = start_y;
    a[x][y] = 1;
 
    for (int i = 0; i < 63; i++)
        if (nextMove(a, &x, &y) == 0)
            return false;

    if (!neighbour(x, y, start_x, start_y))
        return false;
 
    print(a);
    return true;
}

int main(){
	srand(time(0));
    int start_X[2] = {0, 7};
    int start_Y[2] = {0, 7};
    int m = rand()%2;
    int n = rand()%2;
    
	while(Tour(n , m) == false){
    ;}
	
	return 0;
}
