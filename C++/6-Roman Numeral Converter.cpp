/*
  Roman Numeral Converter
  Author: Seyed Sobhan Hosseini
  Date: Tuesday, May 30, 2023, 8:55:44 PM
*/


#include <iostream>
using namespace std;

string ArabicToRoman(int arabic_number){
	    string roman_number;
	    int temp;
			if(arabic_number >= 1000){
			temp = (arabic_number / 1000);
			for(int i = 1; i <= temp ; i++){
				roman_number += 'M';
			}
			arabic_number = (arabic_number%1000);
		}
		if(arabic_number >= 100){
			temp = (arabic_number / 100);
			if(temp ==9){
				roman_number += "CM";
			}
			else if(temp == 8){
				roman_number += "DCCC";
			}
			else if(temp == 7){
				roman_number += "DCC";
			}
			else if(temp == 6){
				roman_number += "DC";
			}
			else if(temp == 5){
				roman_number += "D";
			}
			else if(temp == 4){
				roman_number += "CD";
			}
			else if(temp == 3){
				roman_number += "CCC";
		   }
		    else if(temp == 2){
				roman_number += "CC";
			}
			else if(temp == 1){
				roman_number += "C";
			}
			arabic_number = (arabic_number % 100);
	}
	if(arabic_number >= 10){
			temp = (arabic_number / 10);
			if(temp ==9){
				roman_number += "XC";
			}
			else if(temp == 8){
				roman_number += "LXXX";
			}
			else if(temp == 7){
				roman_number += "LXX";
			}
			else if(temp == 6){
				roman_number += "LX";
			}
			else if(temp == 5){
				roman_number += "L";
			}
			else if(temp == 4){
				roman_number += "XL";
			}
			else if(temp == 3){
				roman_number += "XL";
		   }
		    else if(temp == 2){
				roman_number += "XX";
			}
			else if(temp == 1){
				roman_number += "X";
			}
			arabic_number = (arabic_number % 10);
        }
    if(arabic_number > 0){
            temp = arabic_number;
        if(temp == 9){
            roman_number += "IX";
        }
        else if(temp == 8){
            roman_number += "VIII";
        }
        else if(temp == 7){
            roman_number += "VII";
        }
        else if(temp == 6){
            roman_number += "VL";
        }
        else if(temp == 5){
            roman_number += "V";
        }
        else if(temp == 4){
            roman_number += "IV";
        }
        else if(temp == 3){
            roman_number += "III";
        }
        else if(temp == 2){
            roman_number += "II";
        }
        else if(temp == 1){
            roman_number += "I";
        }
    }
    return roman_number;
}

int main(){
	int arabic_number;
	int temp;
	string roman_number;
	cout << "Please enter your number between <1> and <4999> : " << endl;
	cin >> arabic_number;
	if (arabic_number > 4999 || arabic_number < 1){
		cout << "This number can't be calculated ; Please try again." << endl;
	}
	else{
        cout << "The Roman number is : " << ArabicToRoman(arabic_number) << endl;;
    }
   return 0;	
}
