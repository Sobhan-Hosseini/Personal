/*
  Rectangles
  Author: Seyed Sobhan Hosseini
  Date: Friday, September 29, 2023, 2:08:12 PM
*/

#include <iostream>

using namespace std;

class Rectangle{
	public:
	//attributes
	int tlx;
	int tly;
	int brx;
	int bry;

	//methodes
	void setVals();
	void show();
};

void Rectangle::show(){
		cout << "(" << tlx << " , " << tly << ")";
		cout << " --> ";
		cout << "(" << brx << " , " << bry << ")"; 
	}

void Rectangle::setVals(){
		cout << "Enter the top left x value : ";
		cin >> tlx;
		cout << "Enter the top left y value : ";
		cin >> tly;
		cout << "Enter the bottom right x value : ";
		cin >> brx;
		cout << "Enter the bottom right y value : ";
		cin >> bry;
	}

int main(){
	Rectangle first;
	Rectangle second;
	Rectangle resault;
	cout << "frist rectangle : " << endl;
	first.setVals();
	cout << endl;
	cout << "second rectangle : " << endl;
	second.setVals();
	
	if(second.tlx > first.tlx){
		resault.tlx = second.tlx;
	}else{
		resault.tlx = first.tlx;
	}
	
	if(second.tly < first.tly){
		resault.tly = second.tly;
	}else{
		resault.tly = first.tly;
	}
	
	if(second.brx < first.brx){
		resault.brx = second.brx;
	}else{
		resault.brx = first.brx;
	}
	
	if(second.bry > first.bry){
		resault.bry = second.bry;
	}else{
		resault.bry = first.bry;
	}
	
	system("cls");
	
	cout << "RESAULT :" << endl;
	first.show();
	cout << " && ";
	second.show();
	cout << " == ";
	
	if(resault.tlx > resault.brx || resault.tly < resault.bry){
		cout << "no intersection" << endl;
	}else{
		resault.show();	
	}
	
	return 0;
}
