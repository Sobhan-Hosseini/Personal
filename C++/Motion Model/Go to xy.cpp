/*
  Go to xy
  Author: Seyed Sobhan Hosseini
  Date: Tuesday, August 15, 2023, 6:45:49 PM
*/

#include <iostream>
#include <bits/stdc++.h>
#include <math.h>

using namespace std;

double Convert(double angle){
    double pi = 3.14159265359;
    return (angle * (pi / 180)); 
}

double distance(double x, double y, double des_x, double des_y){
    return sqrt(pow(des_x - x, 2) + pow(des_y - y, 2) * 1.0);
}

int main(){
    double angle = 0;
//set.........................
    double velocity = 5;
    double des_x = 1000;
    double des_y = 700;
//..............................
    double x = 0;
    double y = 0;
	double L = 0.265;
	double delta_x = 0;
	double delta_y = 0;  
	double delta = 0;
	double theta = ((((velocity/100.0) * tan(delta)) / L) * 0.1);
	double delta_theta = 0;
	double dis = distance(x, y, des_x, des_y);
//PID...........................
	double Kp = 1;
	double Ki = 0;
	double Kd = 0;
	double error = atan((des_y - y) / (des_x - x));
	double last_error = 0;
//	double value = 0;
//..............................	
	while(dis > 1){
		if(des_x <= 0){
			error = 3.14159 - (theta - (atan((des_y - y) / (des_x - x))));
		}else{
			error = (atan((des_y - y) / (des_x - x))) - theta;
		}
		
		if(des_x == 0 && des_y < 0){
			error = (-theta - (atan((des_y - y) / (des_x - x)))) - 2.35619;
		}
			
		delta = (Kp * error) + (Kd * (error - last_error));	
		
		if(delta > 0.5236){
			delta = 0.5236;
		}else if(delta < -0.5236){
			delta = -0.5236;
		}
		
		theta = ((((velocity/100.0) * tan(delta)) / L) * 0.5) + theta;			
		x = (((velocity * (cos(theta))) * 0.5) + x);
		y = (((velocity * (sin(theta))) * 0.5) + y);
		
		dis = distance(x, y, des_x, des_y);
		
		last_error = error;
//        value = value + error;
		cout << "x : " << x <<" | " << "y : " << y << " | " << "theta : " << theta * 57.29<< endl;
		}
	return 0;	
}
