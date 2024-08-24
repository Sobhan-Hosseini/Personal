/*
  dynamic memory allocation
  Author: Seyed Sobhan Hosseini
  Date: Summer 2023
*/
#include <iostream>
#include <cstring>
using namespace std;

int main(){
    char **list = nullptr;
    int row = 0;

    while(true){
        cout << "Enter a word (type '0' to exit) : ";
        char input[100];
        cin >> input;

        if(input[0] == '0'){
            break;
        }

        int length = strlen(input);
        char *newWord = new char[length+1];

        strcpy(newWord, input);

        char **newList = new char*[row+1];

        for(int i = 0; i < row; i++){
            newList[i] = list[i];
        }

        newList[row] = newWord; 

        delete[] list;
        list = newList; 
        row++;

        cout << "Output -> " << input << ", Input -> ";
        for(int i = 0; i < length; i++){
            cout << "'" << input[i] << "' ";
        }
        cout << "'\\0'" << endl;
    }

    for (int i = 0; i < row; i++) {
        delete[] list[i];
    }

    delete[] list;

    return 0;
}

