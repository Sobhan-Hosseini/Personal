/*
  Movie Recommender System
  Author: Seyed Sobhan Hosseini
  Date: Saturday, August 12, 2023, 3:45:19 PM
*/

#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <stdexcept>

using namespace std;
//Show the main menu *
void main_menu(){
	int choice;
	system("CLS");
	HANDLE col;
    col =  GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(col, 15);
	cout << "      MOVIE RECOMMENDATION SYSTEM " << endl;
	cout << endl;
	cout << "********WELCOME TO THE MAIN MENU********" << endl;
	cout << "\n\n 1.Display movies \n 2.Display ratings \n 3.Display similarity between two usres" << endl; 
	cout << " 4.Generate recommendations \n 5.Rate a movie \n 6.Add/Remove a movie \n 7.Exit" << endl;
	cout << "\n\nEnter your choice : ";
}
//Display movies **
void display_movies(){
	HANDLE col;
    col =  GetStdHandle(STD_OUTPUT_HANDLE);
    
	ifstream inputFile("Movies.txt");

    if (inputFile.is_open()) {
        vector<string> column1;
        vector<string> column2;
        vector<string> column3;

        string line;
        while (getline(inputFile, line)){
            istringstream iss(line);
            string columnValue;
            int colNum = 0;
            while (getline(iss, columnValue, '\t') && colNum < 3){
                if (colNum == 0){
                    column1.push_back(columnValue);
                } else if (colNum == 1){
                    column2.push_back(columnValue);
                } else if (colNum == 2){
                    column3.push_back(columnValue);
                }
                colNum++;
            }
        }
        inputFile.close();

        int lengths[column2.size()];
        for (size_t i = 0; i < column2.size(); ++i) {
            lengths[i] = column2[i].length();
        }
        
        cout << "----------------" << endl;
        cout << " Display Movies " << endl;
        cout << "----------------" << endl;
        cout << "\n\n";
        cout << " Movie id       Movie name                                           Year" << endl;
        cout << "----------     -----------------------------------------------      ------" << endl;
        
        for (size_t i = 0; i < column1.size(); ++i) {
            cout << " " << column1[i] << "\t\t";
            cout << column2[i];
            cout << setw(57 - lengths[i]) <<  column3[i] << endl;
        }
        string choice_c1;
        cout << "\nPress 'm' to Main menu, and 'q' to Quit : ";
        SetConsoleTextAttribute(col, 2);
        while(true){
        cin >> choice_c1;
        if (choice_c1 == "q" || choice_c1 == "Q"){
        	SetConsoleTextAttribute(col, 15);
            exit (0);
        } else if (choice_c1 == "m" || choice_c1 == "M"){
        	return;
		} else{
			SetConsoleTextAttribute(col, 4);
            cout << "This option is invalid. Try again : ";
            SetConsoleTextAttribute(col, 2);
		    }
        }
        } else{
            cout << "Unable to open the file." << endl;
    }
}
//Display ratings ***
int display_ratings(){
 	while(true){
		HANDLE col;
	    col =  GetStdHandle(STD_OUTPUT_HANDLE);
		string choice_c2;
		
	    ifstream inputFile("Ratings.txt");
	    if (inputFile.is_open()){
	        vector<string> column1;
	        vector<string> column2;
	        vector<string> column3;
	
	        string line;
	        while (getline(inputFile, line)){
	            istringstream iss(line);
	            string columnValue;
	            int colNum = 0;
	            while (getline(iss, columnValue, '\t') && colNum < 3){
	                if (colNum == 0){
	                    column1.push_back(columnValue);
	                } else if (colNum == 1){
	                    column2.push_back(columnValue);
	                } else if (colNum == 2){
	                    column3.push_back(columnValue);
	                }
	                colNum++;
	            }
	        }
	        inputFile.close(); 
	        
		cout << " Display Ratings" << endl;
		cout << "-----------------\n" << endl;
		cout << "Enter the user id : ";
		while(true){
		    SetConsoleTextAttribute(col, 2);
		    cin >> choice_c2;
		    while(stoi(choice_c2) > 50 || stoi(choice_c2) < 1){
		    	SetConsoleTextAttribute(col, 4);
				cout << "This user id does not exist. Try again : ";
				SetConsoleTextAttribute(col, 2);
				cin >> choice_c2;
				SetConsoleTextAttribute(col, 15);
				} 
			    SetConsoleTextAttribute(col, 15);
			    cout << endl;
			    cout << "Ratings of user " << choice_c2 << " are : \n" << endl;
				cout << " Movie id        Rating" << endl;
				cout << "----------      --------" << endl;		 
	            for (size_t i = 0; i < column1.size(); ++i){
	     	        if(column1[i] == choice_c2){
	                    cout << "    " << column2[i] << "\t\t";
	                    cout << setw(4) << column3[i] << endl;
	                    }
	                }
	                
	    string choice_c22;
	    cout << endl; 
	    cout << "Press 'r' to Retry, 'm' to Main menu, and 'q' to Quit : ";
	    SetConsoleTextAttribute(col, 2);
	    while(true){
	        cin >> choice_c22;
	        if (choice_c22 == "q" || choice_c22 == "Q"){
	        	SetConsoleTextAttribute(col, 15);
	            exit (0);
	        } else if (choice_c22 == "m" || choice_c22 == "M"){
	        	return 0;
			} else if (choice_c22 == "r" || choice_c22 == "R"){
				system("CLS");
				break;
			} else{
				SetConsoleTextAttribute(col, 4);
	            cout << "This option is invalid. Try again : ";
	            SetConsoleTextAttribute(col, 2);
			            }
	                }
	        SetConsoleTextAttribute(col, 15);
	        break;
			}
	
	    }else{
	        cout << "Unable to open the file." << endl;
	        return 1;
		}
	}
	return 0;
}  

//Similarity between tow users ****
int sim(){ 
 	while(true){
		HANDLE col;
	    col = GetStdHandle(STD_OUTPUT_HANDLE);
	    
	    ifstream inputFile("Ratings.txt");
	
	    if (inputFile.is_open()){
	        vector<string> column1;
	        vector<string> column2;
	        vector<string> column3;
	
	        string line;
	        while (getline(inputFile, line)){
	            istringstream iss(line);
	            string columnValue;
	            int colNum = 0;
	            while (getline(iss, columnValue, '\t') && colNum < 3){
	                if (colNum == 0){
	                    column1.push_back(columnValue);
	                } else if (colNum == 1){
	                    column2.push_back(columnValue);
	                } else if (colNum == 2){
	                    column3.push_back(columnValue);
	                }
	                colNum++;
	            }
	        }
	        inputFile.close();
	        
	        int sum_sorat = 0;
	        int sum_makhraj1 = 0;
			int sum_makhraj2 = 0;
	        string user_choice1;
	        string user_choice2;
	        
	        cout << "--------------------------------------" << endl;
	        cout << " Display similarity between two users" << endl;
	        cout << "--------------------------------------\n" << endl;
	        cout << "Enter first user id : ";
	        while(true){
	        SetConsoleTextAttribute(col, 2);
	        cin >> user_choice1;
	        SetConsoleTextAttribute(col, 15);
	        while(stoi(user_choice1) <= 0 || stoi(user_choice1) > 50){
	        	SetConsoleTextAttribute(col, 4);
	        	cout << "This user id does not exist. Try again : ";
	        	SetConsoleTextAttribute(col, 2);
				cin >> user_choice1;
				SetConsoleTextAttribute(col, 15);
			}
				cout << "\nEnter second user id : ";
				while(true){
	            SetConsoleTextAttribute(col, 2);
	            cin >> user_choice2;
	            SetConsoleTextAttribute(col, 15);
			        while(stoi(user_choice2) <= 0 || stoi(user_choice2) > 50 || stoi(user_choice2) == stoi(user_choice1)){
			        	SetConsoleTextAttribute(col, 4);
			        	cout << "This user id does not exist. Try again : ";
			        	SetConsoleTextAttribute(col, 2);
						cin >> user_choice2;
						SetConsoleTextAttribute(col, 15);
			        }
					for(int i = 0 ; i < column1.size() ; i++){
		        	    for(int j = 0 ; j < column1.size() ; j++){
		        		    if(column1[i] == user_choice1 && column1[j] == user_choice2){
		        			    if(column2[i] == column2[j]){
		        				    int a = stoi(column3[i]);
		        				    int b = stoi(column3[j]);
		        				    sum_sorat = sum_sorat + (a*b);
		        				    sum_makhraj1 = sum_makhraj1 + (a*a);
		        					sum_makhraj2 = sum_makhraj2 + (b*b);
								}
							}
		        		}
					}
		
				float sim = sum_sorat/(sqrt(sum_makhraj1) * sqrt(sum_makhraj2));
				cout << endl;
				cout << "Similarity between user " << user_choice1 << " and " << user_choice2 << " is : " << setprecision(2) << sim << endl;
				
				string choice_c3;
		        cout << endl; 
		        cout << "Press 'r' to Retry, 'm' to Main menu, and 'q' to Quit : ";
		        SetConsoleTextAttribute(col, 2);
		        while(true){
		        	cin >> choice_c3;
		        	if (choice_c3 == "q" || choice_c3 == "Q"){
		        		SetConsoleTextAttribute(col, 15);
		            	exit (0);
		        	} else if (choice_c3 == "m" || choice_c3 == "M"){
		        		return 0;
					} else if (choice_c3 == "r" || choice_c3 == "R"){
						system("CLS");
						break;
					} else{
						SetConsoleTextAttribute(col, 4);
		            	cout << "This option is invalid. Try again : ";
		            	SetConsoleTextAttribute(col, 2);
				        }
		            }
				SetConsoleTextAttribute(col, 15);
				break;
		    }
		SetConsoleTextAttribute(col, 15);
		break;
	}
		} else {
		    cout << "Unable to open the file." << endl;
		    return 1;
		}
	}
	return 0;
}

//Generate recommendations *****
int gen(){
	while(true){ 
	HANDLE col;
    col = GetStdHandle(STD_OUTPUT_HANDLE);
    
    struct User{
    int id;
    float sim;
    };
    
    struct Hamsaye {
    int id;
    float sim;
    };
    
    struct Final_movies {
    	int id;
    	float pishbini;
	};
    
    const int numUsers = 50;
    User users[numUsers];
    Hamsaye hamsaye[9];
    
    int sum_sorat = 0;
    int sum_makhraj1 = 0;
	int sum_makhraj2 = 0;
    string user_choice1;
    
    vector<string> movies;
    vector<string> unwatched;
    
    vector<string> column1;
    vector<string> column2;
    vector<string> column3;
    vector<string> column4;
    vector<string> column5;
    vector<string> column6;
    
    vector<int> simham;
    
    vector<float> themovies;
	Final_movies final_movies[15];
    
    ifstream inputFile("Ratings.txt");

    if (inputFile.is_open()){
        string line;
        while(getline(inputFile, line)){
            istringstream iss(line);
            string columnValue;
            int colNum = 0;
            while (getline(iss, columnValue, '\t') && colNum < 3){
                if (colNum == 0){
                    column1.push_back(columnValue);
                } else if (colNum == 1){
                    column2.push_back(columnValue);
                } else if (colNum == 2){
                    column3.push_back(columnValue);
                }
                colNum++;
            }
        }
        inputFile.close();
        
	    ifstream inputFile("Movies.txt");
	    if (inputFile.is_open()) {
	        string line;
	        while (getline(inputFile, line)){
	            istringstream iss(line);
	            string columnValue;
	            int colNum = 0;
	            while (getline(iss, columnValue, '\t') && colNum < 3){
	                if (colNum == 0){
	                    column4.push_back(columnValue);
	                } else if (colNum == 1){
	                    column5.push_back(columnValue);
	                } else if (colNum == 2){
	                    column6.push_back(columnValue);
	                }
	                colNum++;
	            }
	        }
	        inputFile.close();
          
        cout << "--------------------------" << endl;
        cout << " Generate Recommendations " << endl;
        cout << "--------------------------\n" << endl;
        cout << "Enter user id : ";
        while(true){
        wtf : 
        SetConsoleTextAttribute(col, 2);
        cin >> user_choice1;
        SetConsoleTextAttribute(col, 15);
	        while(stoi(user_choice1) <= 0 || stoi(user_choice1) > 50){
	        	SetConsoleTextAttribute(col, 4);
	        	cout << "This user id does not exist. Try again : ";
				SetConsoleTextAttribute(col, 2);
				cin >> user_choice1;
				SetConsoleTextAttribute(col, 15);	
			}
			for(int z = 1; z <51 ; z++){
				for(int i = 0 ; i < column1.size() ; i++){
					if(column1[i] == user_choice1){
        	    		for(int j = 0 ; j < column1.size() ; j++){
        		    		if(column1[i] == user_choice1 && stoi(column1[j]) == z){
        			    		if(column2[i] == column2[j]){
        				    		int a = stoi(column3[i]);
        				    		int b = stoi(column3[j]);
        				    		sum_sorat = sum_sorat + (a*b);
        				    		sum_makhraj1 = sum_makhraj1 + (a*a);
        							sum_makhraj2 = sum_makhraj2 + (b*b);
								}
							}
        				}
					}
				}
			float similarity = sum_sorat/(sqrt(sum_makhraj1) * sqrt(sum_makhraj2));
			users[z - 1].id = z;
			users[z - 1].sim = similarity;
			sum_sorat = 0;
			sum_makhraj1 = 0;
			sum_makhraj2 = 0;
    		}
		
	int maxAgeIndex = 0;
    for (int i = 1; i < numUsers; ++i) {
        if (users[i].sim > users[maxAgeIndex].sim)
            maxAgeIndex = i;
    }

    for (int i = 0; i < 9; ++i) {
        hamsaye[i].id = users[maxAgeIndex].id;
        hamsaye[i].sim = users[maxAgeIndex].sim;
        users[maxAgeIndex].sim = 0;

        for (int j = 0; j < numUsers; ++j) {
            if (users[j].sim > users[maxAgeIndex].sim)
                maxAgeIndex = j;
        }
    }

    for(int y = 0; y < column1.size(); y++){
    	if(column1[y] == user_choice1 ){
    		movies.push_back(column2[y]);
		}
	}

    bool present[61] = { false };
    for (const string& str : movies) {
        int num = stoi(str);
        
        if (num >= 1 && num <= 60) {
            present[num] = true;
        }
    }

    for (int i = 1; i <= 60; i++) {
        if (!present[i]) {
            string missingStr = to_string(i);
            unwatched.push_back(missingStr);
        }
    }
    
	float sum_ba = 0;
	float sum_pa = 0;
	float pishbini_number = 0;
	int a = 0;
	int e = 0;
	
	for(int i = 0 ; i < unwatched.size() ; i++){                                     
		for(int j = 1; j < 9 ; j++){
			bool flag = 0; 
			a = 0;
			for(int z = 0 ; z < column1.size(); z++){                                                        
				if(hamsaye[j].id == stoi(column1[z]) && unwatched[i] == column2[z]){			 
				   a = stoi(column3[z]); 
				   flag = 1;
				   break;
				}
			}
			float b = 0;
			b = hamsaye[j].sim;
			if(flag == 1){
				sum_ba = sum_ba + (a * b);
				sum_pa = sum_pa + b;
			}
		}
		
			pishbini_number = sum_ba / sum_pa;
           	themovies.push_back(pishbini_number);
           	final_movies[e].id = stoi(unwatched[i]);
           	final_movies[e].pishbini = pishbini_number;
           	sum_ba = 0;
			sum_pa = 0;
           	e = e + 1;
		}

    sort(themovies.rbegin(), themovies.rend());
    
	for(int i = 0; i <themovies.size(); i++ ){
		final_movies[i].pishbini = themovies[i];
		for(int j = 0; j < themovies.size(); j++){
			if(final_movies[j].pishbini == themovies[i]){
				final_movies[i].id = final_movies[j].id;
			}
		}
	}

	    int lengths[column5.size()];
        for (size_t i = 0; i < column5.size(); ++i) {
            lengths[i] = column5[i].length();
        }
        
    cout << "\nTop three recommendations for user " << user_choice1 << " are : " << endl;
    cout << "\nMovie id        Movie name\t\t\t\t\t     Year           Predicted rating" << endl;
    cout << "--------       ------------\t\t\t\t\t    ------\t   ------------------" << endl;
    
    for(int i = 0; i < 3 ; i++){
    	for(int j = 0; j < themovies.size() ; j++){
    		if(themovies[i] == final_movies[j].pishbini){
    			cout << "   " << final_movies[j].id <<"\t\t";
    			for(int w = 1; w < column4.size(); w++){
    				if(stoi(column4[w]) == final_movies[j].id){
    					cout << column5[w];
    					cout << setw(57 - lengths[w]) << column6[w] <<"\t\t";
					}else{
						continue;
					}
				}
				cout << "  " << setprecision(3) << final_movies[j].pishbini << endl;
				}
			}
		}
	while(true){
		cout << endl;
		cout << "====What do you want to do ?====" << endl;
		cout << "a . Display all predictions for this user" << endl;
		cout << "b . Retry\nc . Back to main menu" << endl;
		cout << "d . Exit" << endl;
		cout << "your choice : ";

        string choice_c3;
        SetConsoleTextAttribute(col, 2);
        while(true){
        	cin >> choice_c3;
        	if (choice_c3 == "d" || choice_c3 == "D"){
        		SetConsoleTextAttribute(col, 15);
            	exit (0);
        	} else if (choice_c3 == "c" || choice_c3 == "C"){
        		SetConsoleTextAttribute(col, 15);
        		return 0;
			} else if (choice_c3 == "b" || choice_c3 == "B"){
				system("CLS");
				SetConsoleTextAttribute(col, 15);
				break;
			}else if(choice_c3 == "a" || choice_c3 == "A"){
			    system("CLS");
			    SetConsoleTextAttribute(col, 15);
			    cout << "\nPrediction of unseen movies by user " << user_choice1 << " are : " << endl;
    			cout << "\nMovie id        Movie name\t\t\t\t\t     Year           Predicted rating" << endl;
    			cout << "--------       ------------\t\t\t\t\t    ------\t   ------------------" << endl;
			    for(int i = 0; i < themovies.size() ; i++){
    				for(int j = 0; j < themovies.size() ; j++){
    					if(themovies[i] == final_movies[j].pishbini){
    						cout << "   " << final_movies[j].id <<"\t\t";
    						for(int w = 1; w < column4.size(); w++){
    							if(stoi(column4[w]) == final_movies[j].id){
    								cout << column5[w];
    								cout << setw(57 - lengths[w]) << column6[w] <<"\t\t";
								}else{
						continue;
					}
				}
				cout << "  " << setprecision(3) << final_movies[j].pishbini << endl;
						}
					}
				}
				
						cout << endl;
						cout << "====What do you want to do ?====" << endl;
						cout << "a . Display all predictions for this user" << endl;
						cout << "b . Retry\nc . Back to main menu" << endl;
						cout << "d . Exit" << endl;
						cout << "your choice : ";
						SetConsoleTextAttribute(col, 2);
			} else{
				SetConsoleTextAttribute(col, 4);
            	cout << "This option is invalid. Try again : ";
            	SetConsoleTextAttribute(col, 2);
		            }
                }
                break;
            	}
            	break;
            	SetConsoleTextAttribute(col, 15);
			}
			}else{
				cout << "Unable to open the file." << endl;
			}
		}else{
        cout << "Unable to open the file." << endl;
        return 1;
    	}
	}
	return 0;	
}

//Rate a movie ******
int rate(){
	while(true){
		HANDLE col;
	    col = GetStdHandle(STD_OUTPUT_HANDLE);
	    
		string user_id;
		string movie_id;
		string rating;
		string save;
		string choice_c3;
		string rewrite;
		
		vector<string> movies;
	    vector<string> unwatched;
	    
	    ifstream inputFile("Ratings.txt");
	
		if (inputFile.is_open()){
	        vector<string> column1;
	        vector<string> column2;
	        vector<string> column3;
	
	        string line;
	        while (getline(inputFile, line)){
	            istringstream iss(line);
	       		string columnValue;
	            int colNum = 0;
	            while (getline(iss, columnValue, '\t') && colNum < 3){
	                if (colNum == 0){
	                    column1.push_back(columnValue);
	                } else if (colNum == 1){
	                	column2.push_back(columnValue);
	                } else if (colNum == 2){
	                    column3.push_back(columnValue);
	            }
	            colNum++;
	        }
	    }
		inputFile.close();
	        
	        
		
		cout << "--------------" << endl;
		cout << " Rate a movie" << endl;
		cout << "--------------\n" << endl;
		cout << "Enter the Information Below" << endl;
		cout << "============================" << endl;
		cout << "User ID : ";
		SetConsoleTextAttribute(col, 2);
		cin >> user_id;
		SetConsoleTextAttribute(col, 15);
		cout << "Movie ID : ";
		SetConsoleTextAttribute(col, 2);
		cin >> movie_id;
		SetConsoleTextAttribute(col, 15);
		cout << "Rating(1-5) : ";
		SetConsoleTextAttribute(col, 2);
		cin >> rating;
		SetConsoleTextAttribute(col, 15);
		cout << "============================\n" << endl;
		cout << "Do you want to save(Y/N) ? ";
		SetConsoleTextAttribute(col, 2);
		cin >> save;
	    SetConsoleTextAttribute(col, 15);
	
		if(save == "y" || save == "Y"){
			if(stoi(user_id) > 50 || stoi(user_id) < 1){
				SetConsoleTextAttribute(col, 4);
				cout << "\nThis user id does not exist." << endl;
				SetConsoleTextAttribute(col, 15);
			}else if(stoi(movie_id) > column1.size() || stoi(movie_id) < 1){
				SetConsoleTextAttribute(col, 4);
				cout << "\nThis movie id does not exist." << endl;
				SetConsoleTextAttribute(col, 15);
			}else if(stoi(rating) > 5 || stoi(rating) < 1){
				SetConsoleTextAttribute(col, 4);
				cout << "\nThis rating is not valid. (only between 1 to 5)" << endl;
				SetConsoleTextAttribute(col, 15);
			}else{
				for(int i = 0; i < column1.size(); i++){
					if(column1[i]== user_id && column2[i] == movie_id && column3[i] != rating){
						string first = column1[i].append("\t");
						string second = first.append(column2[i]);
						string third = second.append("\t");
						rewrite = third.append(rating);
						
						string filename = "Ratings.txt";  
		    			int lineNum = i + 1;         
		    			string newLine = rewrite; 
		
		    			ifstream inputFile(filename);
		    			vector<string> lines;
		    			string line;
		
		    			while (getline(inputFile, line)) {
		        			lines.push_back(line);
		    			}
		
		    			inputFile.close();
		
		    			if (lineNum >= 1 && lineNum <= lines.size()) {
		       			 	lines.erase(lines.begin() + lineNum - 1);
		        			lines.insert(lines.begin() + lineNum - 1, newLine);
		    			} else {
		        			return 0;
		    			}
		
		    			ofstream outputFile(filename);
		    			
		    			for (const string& line : lines) {
		        			outputFile << line << endl;
		    			}
		
		    			outputFile.close();
		
		    			cout << "\nThe record was successfully saved." << endl;
		    			
							}else if(column1[i]== user_id && column2[i] == movie_id && column3[i] == rating){
								SetConsoleTextAttribute(col, 4);
								cout << "\nThis record is already exists." << endl;
								SetConsoleTextAttribute(col, 15);
							}
					}
					
				for(int y = 0; y < column1.size(); y++){
		    		if(column1[y] == user_id ){
		    			movies.push_back(column2[y]);
					}
				}
				
			int maxNumber = stoi(column1[0]);
	
		    for (int i = 1; i < column1.size(); ++i) {
		        if (stoi(column1[i]) > maxNumber) {
		            maxNumber = stoi(column1[i]);
		        }
		    }
			
		    bool present[maxNumber + 1] = { false };
		    for (const string& str : movies) {
		        int num = stoi(str);
		        
		        if (num >= 1 && num <= maxNumber) {
		            present[num] = true;
		        }
		    }
		
		    for (int i = 1; i <= maxNumber; i++) {
		        if (!present[i]) {
		            string missingStr = to_string(i);
		            unwatched.push_back(missingStr);
		        }
		    }
		    
		    for(int j = 0; j < unwatched.size(); j++){
		    	if(movie_id == unwatched[j]){
		    		for(int z = 0; z < column1.size() ; z++){
		    			if(column1[z] == user_id && (column2[z] < movie_id && column2[z + 1] > movie_id)){
							string first = column1[z].append("\t");
							string second = first.append(movie_id);
							string third = second.append("\t");
							rewrite = third.append(rating);
							
							string inputFilename = "Ratings.txt";
		    				string tempFilename = "temp.txt";
		
		   				 	ifstream inputFile(inputFilename);
		    				ofstream tempFile(tempFilename);
		
		    				if (!inputFile.is_open()) {
		        				cout << "Failed to open input file." << endl;
		        				return 0;
		    				}
		
		    				if (!tempFile.is_open()) {
		        				cout << "Failed to create temporary file." << endl;
		        				return 0;
		    				}
		
		    				string line;
		    				int lineNumber = 1;
		
		    				while (getline(inputFile, line)) {
		        				tempFile << line << endl;
		
		        				if (lineNumber == (z + 1)) {
		            				tempFile << rewrite << endl;
		        					}
		        				lineNumber++;
		    				}
		    				inputFile.close();
		    				tempFile.close();
		    				
		    				remove(inputFilename.c_str());
		
		    				rename(tempFilename.c_str(), inputFilename.c_str());
		
		    				cout << "\nThe record was successfully saved." << endl;
								}
							}
						}	
					}	
				}
			}	                 	
		}else if(save == "n" || save == "N"){
			}else{
				SetConsoleTextAttribute(col, 4);
				cout << "\nThis option is invalid." << endl;
				SetConsoleTextAttribute(col, 15);
				}
				
			    cout << endl; 
		        cout << "Press 'r' to Retry, 'm' to Main menu, and 'q' to Quit : ";
		        SetConsoleTextAttribute(col, 2);
		        while(true){
		        	cin >> choice_c3;
		        	if (choice_c3 == "q" || choice_c3 == "Q"){
		        		SetConsoleTextAttribute(col, 15);
		            	exit (0);
		        	} else if (choice_c3 == "m" || choice_c3 == "M"){
		        		return 0;
					} else if (choice_c3 == "r" || choice_c3 == "R"){
						system("CLS");
						SetConsoleTextAttribute(col, 15);
						break;
					} else{
						SetConsoleTextAttribute(col, 4);
		            	cout << "This option is invalid. Try again : ";
		            	SetConsoleTextAttribute(col, 2);
				        }
		            }	
	 			}
	return 0;
}
//Add or Remove a movie *******
int addremove(){
	while(true){
		HANDLE col;
	    col = GetStdHandle(STD_OUTPUT_HANDLE);
	
	    string year;
	    int number;
	    int v;
	    
	    string id;
	    string name;
	    string save;
	    string choice;
	    string choice2;
	    string choice3;
	    string idb;
	    
	    vector<string> column1;
		vector<string> column2;
	    vector<string> column3;
	    vector<string> column4;
		vector<string> column5;
	    vector<string> column6;
	    
	    bool vojood = 1;
	    bool vojood2 = 0;
	    bool flag_ratings = 0;
	    string choice_c6;
	    
	    ifstream inputFile("Ratings.txt");
	
		if (inputFile.is_open()){
			string line;
			while (getline(inputFile, line)){
				istringstream iss(line);
				string columnValue;
				int colNum = 0;
				while (getline(iss, columnValue, '\t') && colNum < 3){
					if (colNum == 0){
					    column4.push_back(columnValue);
					} else if (colNum == 1){
					    column5.push_back(columnValue);
					} else if (colNum == 2){
					    column6.push_back(columnValue);
					}
					colNum++;
				}
			}
			inputFile.close();
		
	    ifstream inputFile("Movies.txt");
	
	    if (inputFile.is_open()) {
	        string line;
	        while (getline(inputFile, line)){
	            istringstream iss(line);
	            string columnValue;
	            int colNum = 0;
	            while (getline(iss, columnValue, '\t') && colNum < 3){
	                if (colNum == 0){
	                    column1.push_back(columnValue);
	                } else if (colNum == 1){
	                    column2.push_back(columnValue);
	                } else if (colNum == 2){
	                    column3.push_back(columnValue);
	                }
	                colNum++;
	            }
	        }
	        inputFile.close();
	
	    cout << endl;
	    cout << "====What do you want to do ?====" << endl;
	    cout << "a . Add a new movie" << endl;
	    cout << "b . Remove a movie\nc . Back to main menu" << endl;
	    cout << "d . Exit" << endl;
	    cout << "\nyour choice : ";
	    
	    string choice_c3;
	    SetConsoleTextAttribute(col, 2);
	    while (true) {
	        cin >> choice_c3;
	        if (choice_c3 == "d" || choice_c3 == "D") {
	            SetConsoleTextAttribute(col, 15);
	            exit(0);
	        } else if (choice_c3 == "c" || choice_c3 == "C") {
	            system("CLS");
	            SetConsoleTextAttribute(col, 15);
	            return 0;
	        } else if (choice_c3 == "b" || choice_c3 == "B") {
	        	system("CLS");
	            SetConsoleTextAttribute(col, 15);
	            cout << "----------------" << endl;
	            cout << " Remove a movie" << endl;
	            cout << "----------------\n" << endl;
	            cout << "====Remove by name or ID ?====" << endl;
	            cout << "a . By name " << endl;
	            cout << "b . By ID " << endl;
	            cout << "your choice : ";
	            SetConsoleTextAttribute(col, 2);
	            cin >> choice;
	            SetConsoleTextAttribute(col, 15);
	            if(choice == "a" || choice == "A"){
	            	cin.ignore();
	            	cout << "\nEnter the name of movie : ";
	            	SetConsoleTextAttribute(col, 2);
	            	getline(cin, name);
	            	SetConsoleTextAttribute(col, 15);
					cout << "\nAre you sure you want to delete '" << name <<"' (Y/N)? ";
					SetConsoleTextAttribute(col, 2);
					cin >> choice2;
	            	SetConsoleTextAttribute(col, 15);
	            	
	            	for(int i = 0; i < column1.size(); i++){
	            		if(name == column2[i]){
	                        vojood2 = 1;
	                        v = i + 1;
	                        id = column1[i];
							}else{
							    continue;
							}
						}
	            	
	            	if(choice2 == "y" || choice2 == "Y"){
	            		if(vojood2 == 1){
	            			string filename = "Movies.txt";
	    					int lineToDelete = v;
	
	    					ifstream inFile(filename);
	    					if (!inFile.is_open()) {
	        					cerr << "Failed to open file: " << filename << endl;
	        					return 0;
	    					}
	
	    					ofstream tempFile("temp.txt");
	    					if (!tempFile.is_open()) {
	        					cerr << "Failed to create temporary file." << endl;
	        					inFile.close();
	        					return 0;
	    					}
	
	    					string line;
	    					int lineNumber = 1;
	
	    					while (getline(inFile, line)) {
	        					if (lineNumber == lineToDelete) {
	            					lineNumber++;
	            					continue;
	       					 	}
	
	        					tempFile << line << endl;
	        					lineNumber++;
	    					}
	
	    					inFile.close();
	    					tempFile.close();
	
						    remove(filename.c_str());
						    rename("temp.txt", filename.c_str());
						    
						    cout << "\nThe record was successfully removed." << endl;
	
	                        for(int z = 0; z < column5.size(); z++){
	                        	if(column5[z] == id){
	                        		string first = column4[z];
	                        		string second = first.append("\t");
	                        		string third = second.append(column5[z]);
	                        		string fourth = third.append("\t");
	                        		string fifth = fourth.append(column6[z]);
	                        		
									string filename = "Ratings.txt";
									string searchLine = fifth; 
										    
									ifstream inputFile(filename);
									if (!inputFile) {
										cerr << "Error opening input file." << endl;
								        return 0;
								    }
										
								    ofstream outputFile("temp.txt");
								    if (!outputFile) {
								        cerr << "Error creating temporary output file." << endl;
									        return 0;
								    }
								
								    string line;
	
									while (getline(inputFile, line)) {
								        if (line != searchLine) {
								            outputFile << line << endl; 
								        }
								    }
	
									inputFile.close();
								    outputFile.close();
						
								    if (remove(filename.c_str()) != 0) {
								        cerr << "Error deleting the input file." << endl;
								        return 0;
									}
										
									if (rename("temp.txt", filename.c_str()) != 0) {
								        cerr << "Error renaming the temporary file." << endl;
								        return 0;
								    }
	                            }
		                    }
						    flag_ratings = 1;  
						}else{
							SetConsoleTextAttribute(col, 4);
	            			cout << "\nThis movie does not exist." << endl;
	            			SetConsoleTextAttribute(col, 15);
						}		
					}else if(choice2 == "n" || choice2 == "N"){
					}else{
						SetConsoleTextAttribute(col, 4);
	            		cout << "\nThis option is invalid.\n";
	            		SetConsoleTextAttribute(col, 15);
					}	
				}else if(choice == "b" || choice == "B"){
					cin.ignore();
	            	cout << "\nEnter movie ID : ";
	            	SetConsoleTextAttribute(col, 2);
	            	cin >> idb;
	            	SetConsoleTextAttribute(col, 15);
					cout << "\nAre you sure you want to delete 'movie " << idb <<"' (Y/N)? ";
					SetConsoleTextAttribute(col, 2);
					cin >> choice3;
	            	SetConsoleTextAttribute(col, 15);
	            	
	            	for(int i = 0; i < column1.size(); i++){
	            		if(idb == column1[i]){
	                        vojood2 = 1;
	                        v = i + 1;
							}else{
							    continue;
							}
						}
						
	            	if(choice3 == "y" || choice3 == "Y"){
	            		if(vojood2 == 1){
	            			string filename = "Movies.txt";
	    					int lineToDelete = v;
	
	    					ifstream inFile(filename);
	    					if (!inFile.is_open()) {
	        					cerr << "Failed to open file: " << filename << endl;
	        					return 0;
	    					}
	
	    					ofstream tempFile("temp.txt");
	    					if (!tempFile.is_open()) {
	        					cerr << "Failed to create temporary file." << endl;
	        					inFile.close();
	        					return 0;
	    					}
	
	    					string line;
	    					int lineNumber = 1;
	
	    					while (getline(inFile, line)) {
	        					if (lineNumber == lineToDelete) {
	            					lineNumber++;
	            					continue;
	       					 	}
	        					tempFile << line << endl;
	        					lineNumber++;
	    					}
	
	    					inFile.close();
	    					tempFile.close();
	
						    remove(filename.c_str());
						    rename("temp.txt", filename.c_str());
						    
						    cout << "\nThe record was successfully removed." << endl;
						    
						    for(int z = 0; z < column4.size(); z++){
	                        	if(column5[z] == idb){
	                        		string first = column4[z];
	                        		string second = first.append("\t");
	                        		string third = second.append(column5[z]);
	                        		string fourth = third.append("\t");
	                        		string fifth = fourth.append(column6[z]);
	                        		
									string filename = "Ratings.txt";
									string searchLine = fifth; 
										    
									ifstream inputFile(filename);
									if (!inputFile) {
										cerr << "Error opening input file." << endl;
								        return 0;
								    }
										
								    ofstream outputFile("temp.txt");
								    if (!outputFile) {
								        cerr << "Error creating temporary output file." << endl;
									    return 0;
								    }
								
								    string line;
	
									while (getline(inputFile, line)) {
								        if (line != searchLine) {
								            outputFile << line << endl; 
								        }
								    }
	
									inputFile.close();
								    outputFile.close();
						
								    if (remove(filename.c_str()) != 0) {
								        cerr << "Error deleting the input file." << endl;
								        return 0;
									}
										
									if (rename("temp.txt", filename.c_str()) != 0) {
								        cerr << "Error renaming the temporary file." << endl;
								        return 0;
								    }
	                            }
		                    }
						    flag_ratings = 1;      
						}else{
							SetConsoleTextAttribute(col, 4);
	            			cout << "\nThis movie does not exist." << endl;
	            			SetConsoleTextAttribute(col, 15);
						}		  		
	            	}else if(choice3 == "n" || choice3 == "N"){
					}else{
						SetConsoleTextAttribute(col, 4);
	            		cout << "\nThis option is invalid.\n";
	            		SetConsoleTextAttribute(col, 15);					
					}	
				}else{
					SetConsoleTextAttribute(col, 4);
	            	cout << "\nThis option is invalid.\n";
	            	SetConsoleTextAttribute(col, 15);
				}          
	        } else if (choice_c3 == "a" || choice_c3 == "A") {
	            system("CLS");
	            SetConsoleTextAttribute(col, 15);;
	            cout << "-----------------" << endl;
	            cout << " Add a new movie" << endl;
	            cout << "-----------------\n" << endl;
	            cout << "Enter the information below" << endl;
	            cout << "===========================" << endl;
	            cin.ignore();
	            cout << "Name : ";
	            SetConsoleTextAttribute(col, 2);
	            getline(cin, name);
	            SetConsoleTextAttribute(col, 15);
	            cout << "Year : ";
	            SetConsoleTextAttribute(col, 2);
	            cin >> year;
	            SetConsoleTextAttribute(col, 15);
	            cout << "===========================\n" << endl;
	            cout << "Do you want to save (Y/N)? ";
	            SetConsoleTextAttribute(col, 2);
	            cin >> save;
	            SetConsoleTextAttribute(col, 15);
	                        
	            if(save == "y" || save == "Y"){
	            	for(int i = 0; i < column1.size(); i++){
	            		if(name == column2[i]){
	            			SetConsoleTextAttribute(col, 4);
	            			cout << "\nThis movie already exists." << endl;
	            			SetConsoleTextAttribute(col, 15);
	            			vojood = 0;
						}else{
							continue;
						}
					}
					if(vojood == 1){
					int lineCount = 0;
					string filename = "Movies.txt";
	
	    			ifstream inputFile(filename);
	    			if (inputFile.is_open()) {
	        			string line;
	
	       			while (getline(inputFile, line)) {
	            		lineCount++;
	        		}
					lineCount = lineCount + 1;
	        		inputFile.close();
					}
					string first = to_string(lineCount);
					string second = first.append("\t");
					string third = second.append(name);
					string fourth = third.append("\t");
					string fifth = fourth.append(year);
					
					ofstream outputFile;
	    			outputFile.open("Movies.txt", ios_base::app); 
	
	    			if (outputFile.is_open()) {
	        			outputFile << "\n" <<fifth;
	        			outputFile.close();
	        			cout << "\nThe record was successfully saved." << endl;
	   				} else {
	        			cout << "Failed to open the file." << endl;
	    				}
					}
	       		}else if(save == "n" || save == "N"){
					}else {
	            		SetConsoleTextAttribute(col, 4);
	            		cout << "\nThis option is invalid.\n";
	            		SetConsoleTextAttribute(col, 15);
	        		}
				}else{
					SetConsoleTextAttribute(col, 4);
	            	cout << "\nThis option is invalid.\n";
	            	SetConsoleTextAttribute(col, 15);
				}

	        cout << endl; 
	        cout << "Press 'r' to Retry, 'p' to Previous, 'm' to Main menu, and 'q' to Quit : ";
	        SetConsoleTextAttribute(col, 2);
	        while(true){
	        	cin >> choice_c6;
	        	if (choice_c6 == "q" || choice_c6 == "Q"){
	        		SetConsoleTextAttribute(col, 15);
	            	exit (0);
	        	} else if (choice_c6 == "m" || choice_c6 == "M"){
	        		return 0;
				} else if (choice_c6 == "r" || choice_c6 == "R" || choice_c6 == "P" || choice_c6 == "p"){
					system("CLS");
					SetConsoleTextAttribute(col, 15);
                	break;
				} else{
					SetConsoleTextAttribute(col, 4);
	            	cout << "This option is invalid. Try again : ";
	            	SetConsoleTextAttribute(col, 2);
			            }
	            	}
	            	break;
				}
			}else{
				cout << "Unable to open the file." << endl;
			}
		}else{
			cout << "Unable to open the file." << endl;
		}
	}
	return 0;
}
//Main function ********
int main(){
	bool flag = 0;
	HANDLE col;
	col =  GetStdHandle(STD_OUTPUT_HANDLE);
	try{
	    while(true){
		    string CHOICE;
		    main_menu();
		    while(true){
		        if(flag == 1){
		    		main_menu();
		    		flag = 0;
				}
			   	SetConsoleTextAttribute(col, 2);
			    cin >> CHOICE;
			    int choice = stoi(CHOICE);
			    switch (choice) {
			        case 1:
			        	system("CLS");
			        	SetConsoleTextAttribute(col, 15);
			        	display_movies();
			        	flag = 1;
			            break;
			        case 2:
			        	system("CLS");
			        	SetConsoleTextAttribute(col, 15);
			        	display_ratings();
			        	flag = 1;
			            break;
			        case 3:
			        	system("CLS");
			            SetConsoleTextAttribute(col, 15);
			        	sim();
			        	flag = 1;
			            break;
			        case 4:
			        	system("CLS");
			            SetConsoleTextAttribute(col, 15);
			        	gen();
			        	flag = 1;
			            break;
			        case 5:
			        	system("CLS");
			            SetConsoleTextAttribute(col, 15);
			        	rate();
			        	flag = 1;
			            break;
			        case 6:
			        	system("CLS");
			            SetConsoleTextAttribute(col, 15);
			        	addremove();
			        	flag = 1;
			            break;
			        case 7:
			        	system("CLS");
			        	SetConsoleTextAttribute(col, 15);
						exit(0);
			            break;
			        default:
					    SetConsoleTextAttribute(col, 4);
					    cout << "This option is invalid. Try again : ";
					    SetConsoleTextAttribute(col, 15);          
					    break;
						}
			    	}
			    	break;
				}
	}catch (const invalid_argument& e) {
		cout << endl;
		SetConsoleTextAttribute(col, 4);
        cerr << "Error :\nDear user\nPlease close the app and open it again.\n**BE CAREFULL TO ENTER VALID INTEGERS**" << endl;
        SetConsoleTextAttribute(col, 15);
    }
	return 0;
}
