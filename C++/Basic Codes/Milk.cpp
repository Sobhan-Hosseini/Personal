/*
  Milk
  Author: Seyed Sobhan Hosseini
  Date: Summer 2023
*/

#include <iostream>
using namespace std;

double a[n] = {};
double b[n] = {};
int sum = 0;

int main(){
	int n;
	cin >> n;
	cout << endl;

	for(int i = 0; i < n; i++){
	   cin >> a[i];
	   sum = sum + a[i];   
	}
	cout << endl;

double initial_sum = sum;
    for(int j = n - 1; j >= 0; j--){
    	b[j] =a[j] + (sum - initial_sum)/(j + 1);
    	cout << b[j] << endl;
    	initial_sum = initial_sum - a[j];
	}
	return 0;
}
