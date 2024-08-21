/*
  PID
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, ‎November ‎1, ‎2023, ‏‎11:17:56 AM
*/

#include <iostream>
#include <cmath>

using namespace std;

class Car{
    public:
    //attributes
    int velocity = 0;
    double delta = 0;
    double delta_theta = 0;
    double L = 0;

    int sensor1,seensor2,sensor3;

    double theta = 0;
    double distance = 0;

    bool obstacle_flag = 0;
    int x = 0;

    //methodes
    int simple_road(int theta){
        while(theta < 10){
            velocity = 45;
            delta = 0;
        }
        return 0;
    };

    int obstacleCheck(int sensor2){
        if(sensor2 < 1000){
            obstacle_flag = 1;
            return 1;
        }else{
            obstacle_flag = 0;
            return 0;
        }

    };

    void obstacleAct(){
        distance = distance + 2 * x;
        while(theta > 10){
            obstacle_flag = 1;
        }
        obstacle_flag = 0;
        distance = distance - 2 * x; 
    };

    void low_speed(int velocity){
        velocity = velocity - 10;
    };

    void high_speed(int velocity){
        velocity = velocity + 10;
    };
}

int main(){
    Car car;

    int error = 0;
    int last_error = 0;

    //PID
    int Kp = 0;
    int Ki = 0;
    int Kd = 0;
    
    while(true){
        error = car.theta;
        car.delta = (Kp * error) + (Kd * (error - last_error));
        car.theta = ((((car.velocity) * tan(car.delta)) / car.L)) + car.theta;
        last_error = error;
    }

    cout << "nice!" << endl;
    return 0;
}
