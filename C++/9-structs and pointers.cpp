/*
  structs and pointers
  Author: Seyed Sobhan Hosseini
  Date: Tuesday, May 30, 2023, 8:55:44 PM
*/

#include <iostream>
#include <string>

using namespace std;

struct Person{
    string name;
    Person *father;
    Person *child;
};

void print_tree(Person *person){
    if(person == nullptr){
        return;
    }

    cout << person->name << endl;
    print_tree(person->father);
}

int main(){
    Person *first_person = new Person();
    cout << "Enter the name of the first person : ";
    cin >> first_person->name;
    first_person->father = nullptr;
    first_person->child = nullptr;

    Person *current_person = first_person;
    bool has_heir = true;
    while(has_heir){
        Person *child = new Person();
        cout << "Enter the name of " << current_person->name << "'s child (or 'none' if there is no heir) : ";
        cin >> child->name;

        if(child->name == "none"){
            has_heir = false;
        }else{
            child->father = current_person;
            child->child = nullptr;

            current_person->child = child;
            current_person = child;
        }
    }
    cout << endl;
    print_tree(first_person);

    return 0;
}

