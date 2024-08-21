/*
  Binary Search Tree
  Author: Seyed Sobhan Hosseini
  Date: Saturday, September 30, 2023, 7:40:10 PM
*/

#include <iostream>

using namespace std;

class Node{
	public:
		//attributes
		int data;
		Node *left;
		Node *right;
		
		//constructors
		Node(){
			data = 0;
			left = NULL;
			right = NULL; 
		}
		
		Node(int value){
			data = value;
			left = NULL;
			right = NULL; 
		} 	
};

class BST{
	public:
		//attributes
		Node *root;
		
		//methodes
		bool isEmpty(){
			if(root == NULL){
				return true;
			}else{
				return false; 
			}
		} 
		
		void insert(Node *new_node){
			if(root == NULL){
				root = new_node;
				cout << "Value inserted as root node!" << endl; 
			}else{
				Node *temp = root;
				while(temp != NULL){
					if(new_node->data == temp->data){
						cout << "Value already exist , Insert another value." << endl;
						return;
					}else if((new_node->data < temp->data) && (temp->left == NULL)){
						temp->left = new_node;
						cout << "Value inserted to the left." << endl;
						break;
					}else if(new_node->data < temp->data){
						temp = temp->left;
					}else if((new_node->data > temp->data) && (temp->right == NULL)){
						temp->right = new_node;
						cout << "Value inserted to the right." << endl;
						break;
					}else{
						temp = temp->right;
					}
				}
			}
		}
		
		Node *search(int val){
			if(root == NULL){
				return root;
			}else{
				Node *temp = root;
				while(temp != NULL){
					if(val == temp->data){
						return temp;
					}else if(val < temp->data){
						temp = temp->left;
					}else{
						temp = temp->right;
					}
				}
				return NULL;	
			}
		}
		
		Node *minVal(Node *node){
			Node *current = node;
			while(current->left != NULL){
				current = current->left;
			}
			return current;
		}
		
		Node *deleteNode(Node *root, int val){
			if(root == NULL){
				return NULL;
			}else if(val < root->data){
				root->left = deleteNode(root->left, val);
			}else if(val > root->data){
				root->right = deleteNode(root->right, val);
			}else{
				if(root->left == NULL){
					Node *temp = root->right;
					delete root;
					return temp;
				}else if(root->right == NULL){
					Node *temp = root->left;
					delete root;
					return temp;
				}else{
					Node *temp = minVal(root->right);
					root->data = temp->data;
					root->right = deleteNode(root->right,temp->data);
				}
			}
			return root;
		}
		
		void print(Node *root, int space){
			if(root == NULL){
				return;
			}
			space += 10;
			print(root->right, space);
			cout << endl;
			for(int i = 10; i < space; i++){
				cout << " ";
			}
			cout << root->data << "\n";
			print(root->left, space);
		}
		
		//constructors
		BST(){
			root = NULL;
		}
	
};

int main(){
	int choice, value;
	char any;
	BST object; 
	
	do{
		cout << "==== BST ====" << endl;
		cout << "1.Insert Node" << endl;
		cout << "2.Search Node" << endl;
		cout << "3.Delete Node" << endl;
		cout << "4.Print BST" << endl;
		cout << "5.Exit" << endl;
		cout << endl;
		cout << "your choice : "; 
		cin >> choice;
		system("cls");
		Node *new_node = new Node();
		
		switch(choice){
			case 1:
				cout << "##### INSERT #####" <<endl;
				cout << "Enter the value to insert in BST : " << endl;
				cin >> value;
				new_node->data = value;
				object.insert(new_node);
				cout << endl;
				cout << "Press any key : ";
				cin >> any;
				if(any >= 0 && any <= 127){
					system("cls");
				}
				break;
			case 2:
				cout << "##### SEARCH #####" <<endl;
				cout << "Enter value to search in BST : " << endl;
				cin >> value;
				new_node = object.search(value);
				if(new_node != NULL){
					cout << "Value found." << endl;
				}else{
					cout << "Value NOT found." << endl;
				}
				cout << endl;
				cout << "Press any key : ";
				cin >> any;
				if(any >= 0 && any <= 127){
					system("cls");
				}
				break;
			case 3:
				cout << "##### DELETE #####" <<endl;
				cout << "Enter value to delete in BST : " << endl;
				cin >> value;
				new_node = object.search(value);
				if(new_node != NULL){
					object.deleteNode(object.root, value);
					cout << "Value deleted" << endl;	
				}else{
					cout << "value does NOT exist.";
				}
				cout << endl;
				cout << "Press any key : ";
				cin >> any;
				if(any >= 0 && any <= 127){
					system("cls");
				}
				break;
			case 4 :
				cout << "##### PRINT #####" <<endl;
				object.print(object.root, 0);
				cout << endl;
				cout << "Press any key : ";
				cin >> any;
				if(any >= 0 && any <= 127){
					system("cls");
				}
				break;			
			case 5:
				system("cls");
				break;
		}
	}while(choice != 5);

	return 0;
}
