/*
  Express Number
  Author: Seyed Sobhan Hosseini
  Date: Summer 2023
*/

#include <iostream>

using namespace std;

void ex_not(int n , int count){
	
	int arrexpandedform[count] = {0};
	
	for (int i = 0; i < count; i++){
		unsigned int rem = n % 10;
		
		for (int j = 1; j <= i; j++){
			rem *= 10;
		
		}
                arrexpandedform[i] = rem;
		n = n / 10;
	}
	 for(int i = count - 1; i >= 0; i-- ){
	 	if ( i == 0){
	 		cout << arrexpandedform[i] << endl;
		 }else{
		 	cout << arrexpandedform[i] << " + ";
		 }
	 }
}

int main(){
	int n,x;
    int count = 0;
    
    cout << "Enter the number please : " << endl;
	cin >> n;

	x = n;
  while (x > 0) {
        x = x / 10;
        ++count;
    };
	 
    cout << endl;
	cout << "expand(" << n << ") --> " << endl;
	ex_not(n , count); 
	return 0;
}
