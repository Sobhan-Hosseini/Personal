/*
  Points
  Author: Seyed Sobhan Hosseini
  Date: Friday, September 29, 2023, 8:51:24 PM
*/

#include <iostream>
#include <cmath>

using namespace std;

//line --> Ax + By + C = 0
class Line{
	public:
		//attributes	
		double A;
	    double B;
	    double C;
    
    	//methodes
    	void setVals(){
    		cout << "A : ";
    		cin >> A;
    		cout << "B : ";
    		cin >> B;
    		cout << "C : ";
    		cin >> C;
		}
		
		//constructor
		Line(double A_val = 0, double B_val = 0, double C_val = 0)
			:A {A_val}, B {B_val}, C {C_val} {
			}
		
		//destructor
		~Line(){
		}
		
};

//point --> (x,y)
class Point{
	public:
		//attributes
		double x;
		double y;
	
		//methodes
		void setVals(){
			cout << "x : ";
			cin >> x;
			cout << "y : ";
			cin >> y;
		}
		
		double distanceToPoint(Point otherPoint){
	        double dx = x - otherPoint.x;
	        double dy = y - otherPoint.y;
	        return sqrt(dx*dx + dy*dy);
	    }
		
		double distanceToLine(Line line){
	        double distance = abs(x * line.A + y * line.B + line.C) / sqrt(line.A * line.A + line.B * line.B);
	        return distance;
    	}
		
		void rotate(double angle){
			angle = angle * 3.141592 / 180.0;
	        double newX = x * cos(angle) - y * sin(angle);
	        double newY = x * sin(angle) + y * cos(angle);
	        x = newX;
	        y = newY;
	        cout << "Point new Coordinates : (" << x << ", " << y << ")" << endl;
    	}
		
		void calculateLineEquation(Point otherPoint){
			if(otherPoint.y == y && otherPoint.x == x){
				cout << "NO LINE!" << endl;
			}else{
		        double slope = (otherPoint.y - y) / (otherPoint.x - x);
		        double constant = y - (slope * x);
		        cout << "Line equation : y = " << slope << "x + " << constant << endl;
	    	}
    	}
		
		void calculateLineEquation(double slope){
	        double constant = y - (slope * x);
			cout << "Line equation : y = " << slope << "x + " << constant << endl;
	    }	
};

int main(){
	double angle;
	double slope;
	
	Point firstPoint;
	Point secondPoint;
	Line line;
	
	cout << "Point --> (x , y) && Line --> Ax + By + C = 0" << endl;
	cout << "1)===========================================" << endl;
	cout << "First point : " << endl;
	firstPoint.setVals();
	cout << "Second point : " << endl;
	secondPoint.setVals();
	cout << "Distance : " << firstPoint.distanceToPoint(secondPoint) << endl;;
	cout << "2)===========================================" << endl;
	
	firstPoint.setVals();
	cout << "Enter the angle of rotation : ";
	cin >> angle;
	firstPoint.rotate(angle);
	cout << "3)===========================================" << endl;
	
	cout << "Point : " << endl;
	firstPoint.setVals();
	cout << "Line (Ax + By + C = 0) : " << endl;
	line.setVals();
	cout << "Distance : " << firstPoint.distanceToLine(line) << endl;
	cout << "4)===========================================" << endl;
	
	cout << "First point : " << endl;
	firstPoint.setVals();
	cout << "Second point : " << endl;
	secondPoint.setVals();
	firstPoint.calculateLineEquation(secondPoint);
	cout << "5)===========================================" << endl;
	
	cout << "Point : " << endl;
	firstPoint.setVals();
	cout << "Enter the slope : ";
	cin >> slope;
	firstPoint.calculateLineEquation(slope);
	
	return 0;
}
