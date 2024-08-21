/*
  Circular Movement
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, August 16, 2023, 10:43:00 AM
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
    double velocity = 0;
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
	
	while(flag == true){
	    cout <<"Enter the velocity of the vehicle (max 50) : ";
	    cin >> velocity;
	    cout << "Enter the steer first angle (max 30) : ";
	    cin >> angle; 
	    if(velocity > 50 || velocity < -50 || angle > 30 || angle < -30){
	    	cout << endl;
	    	cout << "this values are out of range !" << endl;
		}else{
			time = 0;
				
		    delta = Convert(angle);
		
		    while(time < 10.5){				
				x = (((velocity * (cos(theta))) * 0.1) + x);
				y = (((velocity * (sin(theta))) * 0.1) + y);
				theta = ((((velocity/100.0) * tan(delta)) / L) * 0.1) + theta;
				
				time = time + 0.1;
			
				cout << "x : " << x <<" | " << "y : " << y << " | " << "theta : " << theta << endl;
//				cout << x << endl;
			}
		}
		cout << "Enter 'Q' if you want to quite and any key if you want to quite : ";
		cin >> choice;
		if(choice == "Q" || choice == "q"){
			flag = false;
		}else{
			flag = true;
		}
	}
	return 0;	
}
