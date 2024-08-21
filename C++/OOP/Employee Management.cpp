/*
  Employee Management
  Author: Seyed Sobhan Hosseini
  Date: Sunday, August 18, 2024, 6:06:18 AM
*/

#include <iostream>
#include <string>

// Base class Employee
class Employee{
protected:
    int employeeID;
    std::string name;
    double baseSalary;

public:
    Employee(int id, std::string empName, double salary)
        : employeeID(id), name(empName), baseSalary(salary){}

    virtual double calculateSalary() const{
        return baseSalary;
    }

    virtual void displayEmployeeInfo() const{
        std::cout << "Employee ID: " << employeeID << ", Name: " << name
                  << ", Base Salary: " << baseSalary << std::endl;
    }

    virtual ~Employee() = default;
};

// Derived class Manager
class Manager : public Employee{
private:
    double bonus;

public:
    Manager(int id, std::string empName, double salary, double bonusAmount)
        : Employee(id, empName, salary), bonus(bonusAmount){}

    double calculateSalary() const override{
        return baseSalary + bonus;
    }

    void displayEmployeeInfo() const override{
        std::cout << "Manager - ";
        Employee::displayEmployeeInfo();
        std::cout << "Bonus: " << bonus << std::endl;
    }
};

// Derived class Engineer
class Engineer : public Employee{
private:
    int overtimeHours;
    double overtimeRate;

public:
    Engineer(int id, std::string empName, double salary, int otHours, double otRate)
        : Employee(id, empName, salary), overtimeHours(otHours), overtimeRate(otRate){}

    double calculateSalary() const override{
        return baseSalary + (overtimeHours * overtimeRate);
    }

    void displayEmployeeInfo() const override{
        std::cout << "Engineer - ";
        Employee::displayEmployeeInfo();
        std::cout << "Overtime Hours: " << overtimeHours
                  << ", Overtime Rate: " << overtimeRate << std::endl;
    }
};

// Derived class Intern
class Intern : public Employee{
private:
    double stipend;

public:
    Intern(int id, std::string empName, double stipendAmount)
        : Employee(id, empName, stipendAmount), stipend(stipendAmount){}

    double calculateSalary() const override{
        return stipend;
    }

    void displayEmployeeInfo() const override{
        std::cout << "Intern - ";
        Employee::displayEmployeeInfo();
        std::cout << "Stipend: " << stipend << std::endl;
    }
};

void processEmployee(Employee* emp){
    emp->displayEmployeeInfo();
    std::cout << "Total Salary: " << emp->calculateSalary() << std::endl;
}

int main(){
    Manager mgr(101, "Alice Smith", 70000.0, 15000.0);
    Engineer eng(102, "Bob Johnson", 60000.0, 10, 50.0);
    Intern intern(103, "Charlie Brown", 1500.0);

    Employee* empPtr;

    empPtr = &mgr;
    processEmployee(empPtr);

    empPtr = &eng;
    processEmployee(empPtr);

    empPtr = &intern;
    processEmployee(empPtr);

    return 0;
}

