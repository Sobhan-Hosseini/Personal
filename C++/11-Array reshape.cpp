/*
  Array reshape
  Author: Seyed Sobhan Hosseini
  Date: Tuesday, May 30, 2023, 8:55:44 PM
*/

#include <iostream>
using namespace std;

int main(){
    int one, two, three;
    cout<<"Enter the Dimension of 3D Array: " << endl;
    cin>>one>>two>>three;
    int a[one][two][three] = {};
    int elements = one*two*three;
    cout<<"Enter "<<elements<<" 3D Array Elements: " << endl;
//get 3d array    
    for(int i = 0; i < one; i++){
        for(int j = 0; j < two; j++){
            for(int k = 0; k < three; k++){
                cin>>a[i][j][k];
            }
        }
    }
cout<<endl;
//show 3d array    
    for(int d = 0; d < one; d++){
        for(int b = 0; b< two; b++){
            for(int c = 0; c < three; c++)
                cout<<"a["<<d<<"]["<<b<<"]["<<c<<"] = "<<a[d][b][c]<<"  ";
            cout<<endl;
        }
        cout<<endl;
    }
    cout << endl;
//convert 3d to 1d
    int b[elements] = {};
    int z = 0;
    while(z < elements){
    	for(int i = 0; i < one; i++){
            for(int j = 0; j < two; j++){
                for(int k = 0; k < three; k++){
                    b[z] = a[i][j][k];
                    z = z + 1;
            }
        }
    }	
}
//show 1d array	
	for(int d = 0; d < elements; d++){
		cout << "b["<< d << "] : " << b[d] << endl;
	}
cout << endl;
cout << endl;	
//convert 1d to 3d
    int c[one][two][three] = {};
    int r = 0;
	while(r < elements){	
	for(int i = 0; i < one; i++){
        for(int j = 0; j < two; j++){
            for(int k = 0; k < three; k++){
               c[i][j][k] = b[r];
               r = r + 1;
            }
        }
    }	
} 
//show again the 3d array
   for(int a = 0; a < one; a++){
        for(int b = 0; b < two; b++){
            for(int d = 0; d < three; d++)
                cout<<"c["<<a<<"]["<<b<<"]["<<d<<"] = "<< c[a][b][d] <<"  ";
            cout<<endl;
        }
    cout<<endl;
    }
cout<<endl;

    return 0;
}
	
