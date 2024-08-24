/*
  Super Digit
  Author: Seyed Sobhan Hosseini
  Date: Summer 2023
*/

#include <iostream>
#include <string>
#include<bits/stdc++.h>
using namespace std;

int getSum(int n){
        int sum = 0;
        while (n != 0){
            sum = sum + n % 10;
            n = n / 10;
        }
    return sum;
}

int SuperDigit(string n, int k){
	string a = n;
	int final_number;
	int sum2 = 0;
	
	int number = stoi(a);
	final_number = k * getSum(number);

    while(final_number > 0 || sum2 > 9){
        if(final_number == 0){
            final_number = sum2;
            sum2 = 0;
        }
        sum2 += final_number % 10;
        final_number /= 10;
    }
    return sum2;
}

int main(){
	string n;
	int k;
	cout << "n : " << endl;
	cin >> n;
	cout << "k : " << endl;
	cin >> k;
	cout << endl;
	cout << "SuperDigit of your number is : " << SuperDigit(n, k) << endl;

    return 0;

}
