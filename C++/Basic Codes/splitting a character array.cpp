/*
  splitting a character array 
  Author: Seyed Sobhan Hosseini
  Date: Summer 2023
*/

#include <iostream>
#include <cstring>
using namespace std;

void reverseWords(char *s){
    char *start = s;
    char *end = s;

    while (*end) {
        end++;
    }
    end--;

    while(start < end){
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    start = end = s;
    while(*start){
        while(*end && *end != ' '){
            end++;
        }
        end--;

        while(start < end){
            char temp = *start;
            *start = *end;
            *end = temp;
            start++;
            end--;
        }
        
        start = end + 2;
        end = start;
    }
}

int main(){
    char sentence[100];

    cout << "Enter your sentence : ";
    cin.getline(sentence, 100);

    reverseWords(sentence);

    cout << "Reversed sentence is : " << sentence << endl;
    
    return 0;
}

