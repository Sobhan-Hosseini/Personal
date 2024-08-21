/*
  Changing Cordinate Frame
  Author: Seyed Sobhan Hosseini
  Date: Tuesday, May 30, 2023, 8:55:44 PM
*/

#include<iostream>
#include <math.h>
#include<bits/stdc++.h>
using namespace std;

float distance(int x, int y){
    return sqrt(pow(x, 2) + pow(y, 2) * 1.0);
}

int main(){
	int x, y, teta;
	int number;
	double xprime, yprime;
	
	cout << "Enter the coordinate of your point : " << endl;
	cout << "x : ";
	cin >> x;
	cout << "y : ";
	cin >> y;
	cout << "Enter the degree of rotation : " << endl;
	cout << "teta : ";
	cin >> teta;
	 	
	number = teta / 360;
	if(number >= 2){
		teta = teta - ((number - 1) * 360);
	}
	
	double pi = 3.14159265359;
	float rad = teta * (pi / 180);
	float D = (distance(x, y));
	double alpha = atan2(y, x);
	
	if(x == 0 && (teta == 90 || teta == 270)){
		yprime = 0;
		xprime = D * cos(alpha + rad); 
	}else if(y == 0 && (teta == 90 || teta == 270)){
		xprime = 0;
		yprime = D * sin(alpha + rad);
	}else if(x == 0 && (teta == 0 || teta == 180)){
		xprime = 0;
		yprime = D * sin(alpha + rad);	
	}else if(y == 0 && (teta == 0 || teta == 180)){
		yprime = 0;
		xprime = D * cos(alpha + rad);	
	}else{
		xprime = D * cos(alpha + rad);
		yprime = D * sin(alpha + rad);	
	}
	
	cout << endl;
	cout << "x after rotation is : ";
	cout << xprime << endl;
	cout << "y after rotation is : ";
    cout << yprime << endl;

	return 0;
}
