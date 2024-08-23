#ifndef MOTION_H
#define MOTION_H

#include "Modules.h"
#include "Actuator.h"
#include <math.h>

#define LATERAL_INTEGRATION_HORIZON 5
#define LONGITUDINAL_INTEGRATION_HORIZON 50

class Motion
{
private: 
protected:
    // double cross_track_error = 0, heading_error = 0, integral_cross_track_error = 0;
    
    //General
    bool go_to_xy_flag = true;
    double distance = 0;
    double last_error_xy = 0;
    double last_distance = 0 ; 
    double kappa = 0;
    double first_angle = 0;
    double theta_kappa = 0;
    double length = 0;
    double first_point = 0;
    double kappa_arr[15] {0};
    int time_kappa = 0;
    int last_time_kappa = 0;
    int base_speed = 50;

    //PID Coefficients
    double Kp = 0.35;   //0.47
    double Ki = 0;  //0.005
    double Kd = 0.2;    //0.6
    double I_buffer[20] {0};
    int sum_CTE = 0;
    int last_error = 0;
   
    //Pure Pursuit Coefficients
    double Kpp = 1.8; //left_coef = 0.75 // kpp = 0.9
    double L = 26.3;

    //Stanley Coefficients
    double K = 0.9; //edit: //0.9 //0.75 //0.7  //race:0.8 //0.7*    //0.7
    double K_HE = 0.5;//edit: //0.5 //45 //0.55 //race:0.6  //0.6*   //0.65
    double Ks = 20;
    double angle_arr[5] {0};
    double distance_arr[5] {0};
    double sum_distance = 0;
    double sum_angle = 0;
    int last_steer = 0;
    int last_HE = 0;
    int counter_HE = 0;
    int last_CTE = 0;
    int counter_CTE = 0;
    bool reset_filter = false;

    
    //Speed Controller
    int angle_buffer[7] {0};
    int loop_number = 0;
    int speed = 0;
    int real_speed = 0;
    int sum_HE = 0;
    double gain = 0.035;    //edit: race : 0.17 - 0.2 ... city : 0.035  //e:
    int counter_speed_controller = 0;
    int last_time_speed = 0;

    //obstacle
    int angle_memory = 0;
    bool angle_memory_reset_status = true;

    //obstacle controller
    int speed_obs = 0;

    //Localization Global
    double dt_global = 0;
    double Kt_global = 5.25;
    double base_theta_global = 0;
    int last_time_global = 0;
    int time_global = 0;
    bool start_flag_global = true;

    //Localization Local
    double x_local,y_local,theta_local = 0;
    double dt_local = 0;
    double Kt_local = 5.526; //5.25
    double base_theta_local = 0;
    int last_time_local = 0;
    int time_local = 0;
    bool start_flag_local = true;

    //Encoder variables
    volatile double last_enc = 0.0;
    volatile double delta_enc = 0.0;
    double last_enc_temp = 0.0;
    double delta_enc_temp = 0.0;
    double last_enc_curve = 0.0;
    bool encoder_reset_status = true;
    bool distance_memory_reset_status = true;
    int traveled_angle = 0;

public:
    //General
    void updater_setup();
    void operate(int _steer, int _speed) ; 
    void stop() ; 
    void calculate_kappa();
    void reset_kappa_parameter(){first_point = 0;};
    int get_base_speed(){return base_speed;};
    int min_of_front_VLs();
    int min_of_back_VLs();
    double convert_angle_to_radian(double angle);
    double mean_kappa();
    double get_kappa(){return kappa;};
    bool go_for_distance(int distance, int _steer, int _speed);
    bool go_for_angle(int angle, int _steer, int _speed);
    bool go_to_xy(double des_x, double des_y, int speed );
    bool go_to_xytheta(double des_x, double des_y, double des_theta, int speed);
    
    //controllers
    int PID();
    int stanley();
    int pure_pursuit();
    int speed_controller(int max_speed);
    int obstacle_controller(int max_speed);
    int go_straight(double gain = 5);

    //Localization
    double x_global,y_global,theta_global = 0;
    void calculat_front_state_global();
    void calculat_back_state_global();
    void calculat_front_state_local();
    void calculat_back_state_local();
    void reset_localization_local();
    void reset_stanley_filter();
    double get_theta_local(){return theta_local;};
    
    //Resets
    bool get_enc_reset_status() { return encoder_reset_status; }
    void set_enc_reset_status(bool input) { encoder_reset_status = input; }
    void set_angle_memory_reset(bool input) { angle_memory_reset_status = input; }

    double not_abs(double input)
    {
        if (input >= 0)
        {
            return input;
        }
        else
        {
            return -input;
        }
    } 
};

#if !defined(NO_GLOBAL_MOTION)
extern Motion motion;
#endif
#endif