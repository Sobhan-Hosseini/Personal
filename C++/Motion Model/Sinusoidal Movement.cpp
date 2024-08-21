/*
  Sinusoidal Movement
  Author: Seyed Sobhan Hosseini
  Date: Sunday, August 20, 2023, 6:30:32 PM
*/

#include <iostream>
#include <bits/stdc++.h>
#include <math.h>

using namespace std;

double Convert(double angle){
    double pi = 3.14159265359;
    return (angle * (pi / 180)); 
}

int main(){
	clock_t start, end;
	bool flag = true;
	string choice;
    double angle = 0;
    double velocity = 50;
    double time = 0;
    double x = 0;
    double y =   0;
	double delta_x = 0;
	double delta_y = 0; 
	double delta_t = 0; 
	double delta = 0;
	double theta =  0;
	double delta_theta = 0;
	double L = 0.265;
	double beta = 0;
	double lr = 0.1325;
	
	while(time < 30){
		while(theta < 1.57){
			delta = Convert(angle);
			x = (((velocity * (cos(theta))) * 0.1) + x);
			y = (((velocity * (sin(theta))) * 0.1) + y);
			theta = ((((velocity/100.0) * tan(delta)) / L) * 0.1) + theta;
					
			time = time + 0.1;
				
//			cout << "x : " << x <<" | " << "y : " << y << " | " << "theta : " << theta << endl;
			cout << x << endl;
			}
			
		while(theta > -1.57){
			angle = -angle;
			delta = Convert(angle);
			x = (((velocity * (cos(theta))) * 0.1) + x);
			y = (((velocity * (sin(theta))) * 0.1) + y);
			theta = ((((velocity/100.0) * tan(delta)) / L) * 0.1) + theta;
					
			time = time + 0.1;
				
//			cout << "x : " << x <<" | " << "y : " << y << " | " << "theta : " << theta << endl;
			cout << x << endl;
			}
		}
	return 0;	
}
