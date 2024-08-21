/*
 XOR cipher
  Author: Seyed Sobhan Hosseini
  Date: Tuesday, May 30, 2023, 8:55:44 PM
*/

#include <iostream>
#include <bits/stdc++.h>
#include <sstream>  
#include <string>
#include <iomanip>
using namespace std;

string hexToBin(string input){
  unsigned int q =  stoul(input, nullptr, 16);
  string result = bitset<16>(q).to_string();
 return result;
}

string xoring(string a, string b){
    string ans = "";
 
    for (int i = 0; i < 16; i++){
        if (a[i] == b[i])
            ans += "0";
        else
            ans += "1";
    }
    return ans;
}

void decToHex(int n){ cout << hex << n;}

int main(){
	string first_num, second_num, longer;
	string shorter;
	string output;
	cout << "Enter the first number : " << endl;
	cin >> first_num;
	cout << "Enter the second number : " << endl;
	cin >> second_num;
	
	if(first_num.length() < second_num.length()){
		shorter = first_num;
		longer = second_num;
	}else if(first_num.length() >= second_num.length()){
		shorter = second_num;
		longer = first_num;
	}else{
		cout << "both" << endl;
	};
	
	int x = shorter.length();
	string premorse[x] = {};
    string rshorter[x] = {};
    
    for(int i = 0; i < x; i++){
    	premorse[i] = longer[i];
	}
	
	for(int j = 0; j < x; j++){
    	rshorter[j] = shorter[j];
	}
	
    string sbinary;
    cout << "\nXOR of these two numbers is : " << endl;
    
	for(int z = 0; z < x; z++){
		string a;
		string b;
        a = hexToBin(rshorter[z]);
		b = hexToBin(premorse[z]);
		
		sbinary = xoring(a,b);

		unsigned long decDigit = stoi(sbinary,nullptr,2);
        decToHex(decDigit);		
	}
	
	return 0;
}
