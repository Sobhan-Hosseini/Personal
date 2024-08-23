/*
  Real-time Mapping for Autonomous Vehicle 
  Authors: Seyed Sobhan Hosseini - Amirhossein Gholizadeh
  Date: Spring 2024
*/

#include "Modules.h"
#include "Actuator.h"
#include "Motion.h"
#include "Cross_Park.h"
#include "Intersection.h"
#include "Parallel_Park.h"
#include "Overtake.h"
#include "Mapping.h"
#include "A_Star.h"

// Characterize ADC at particular atten
// esp_adc_cal_characteristics_t *adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));

// Adafruit_NeoPixel battery(8, BATTERY_LIGHT, NEO_GRB + NEO_KHZ800);
// Adafruit_NeoPixel head(27, REAR_LIGHT, NEO_GRB + NEO_KHZ800);

void System_Init(void *);

void Autonomous(void *);

void setup()
{
  xTaskCreatePinnedToCore(
      System_Init,
      "system initialization",
      4096,
      NULL,
      10,
      NULL,
      APP_CPU_NUM);
  vTaskDelete(0);
}

void System_Init(void *)
{
  modules.module_setup();
  vTaskDelay(100);
  motor.motor_setup();
  steer.steer_setup();

  xTaskCreatePinnedToCore(
      Autonomous,
      "Autonomous",
      32768,
      NULL,
      14,
      NULL,
      APP_CPU_NUM);

  vTaskDelete(0);
}
// 

void Autonomous(void *)
{
  modules.Buzzer(16);
  delay(1000);
  modules.Buzzer(0);
  
  vl.start_vl(VFR | VFMR | VF | VFML | VFL | VR | VL | VBL | VBML | VB | VBMR | VBR);

  bool distance_lock = true;
  bool stop_VL_flag = false;
  bool stop_detected = false;
  int sign_distance = 0;
  int vl_counter = 0;
  double main_kappa = 0;
  int time1 = 0;
  intersection.send_crossWalk_end_flag();

  // mapping.begin_server();
  
  astar.readMap();
  astar.initializeStartPoint(astar.start_x, astar.start_y);
  astar.HybridAstar();
  astar.pathFinding();


  while (true)
  {
    //CODE HELPER
    //edit:
    //delete:
    //race:
    //a:width of road  b:height of crosswalk  c:min distance between intersection  d:height and width of crosswalk
    //e:min Radius of curve f:park box size g:height and width of intersection  h:tunnel  i:trafficlight  j:min distance between park and crosswalk


    // -------Calculate Kappa-------
        motion.calculate_kappa();
    // -----------------------------


    // -----------Mapping-----------
    // -----------------------------
    // motion.operate(0, 20);
    motion.calculat_back_state_global();
    // mapping.mapping();
    astar.instruction();
  
    // uart.debug_print("theta: ");
    // uart.debug_print(motion.theta_global);
    // uart.debug_print("\tx: ");
    // uart.debug_print(motion.x_global);
    // uart.debug_print("\ty:  ");
    // uart.debug_println(motion.y_global);
    // motion.operate(0, 20);

    // if(!motion.go_for_distance(2000, 0, 30)){
    //   motion.stop();
    // }
    // else{
    //   mapping.mapping();
    // }

    // switch(vision.get_sign()){

    //   case vision.BARRED:
    //   {
    //     // overtake.instruction();  //edit : overtake enabale
    //     vision.set_sign(vision.TRAVERSE);
    //     break;
    //   }
      
    //   case vision.CROSSWALK:
    //   {
    //     if(vision.get_distance_from_sign() > 500 && sign_distance == 0){
    //       motion.operate(motion.stanley(), motion.speed_controller(20)); 
    //       main_kappa = motion.mean_kappa();
    //     }
    //     else {
    //       if(distance_lock){
    //         motion.operate(motion.stanley(), motion.obstacle_controller(20));
    //         sign_distance = vision.get_distance_from_sign();
    //         // vTaskDelay(3000);
    //         motor.start_encode();
    //         distance_lock = false;
    //       }
    //       else{
    //         if(motor.get_distance_covered_mm() < sign_distance + (main_kappa * 3600)){
    //           motion.operate(motion.stanley(), motion.obstacle_controller(20));
    //         }
    //         else{
    //           motion.stop();
    //           if(motion.min_of_front_VLs() < 250){
    //             motion.stop();
    //             vTaskDelay(1000);
    //           }
    //           else{
    //             vTaskDelay(5000);
    //             sign_distance = 0;
    //             vision.set_sign(vision.TRAVERSE);
    //             distance_lock = true;
    //           }
    //         }
    //       }
    //     }

    //     uart.debug_println("1");
    //     break;
    //   }

    //   case vision.DOWNHILL:
    //   {
    //     vision.set_sign(vision.TRAVERSE);
    //     uart.debug_println("2");
    //     break;
    //   }

    //   case vision.GIVEWAY:
    //   {

    //     if(vision.get_distance_from_sign() > 400){
    //       motion.operate(motion.stanley(), motion.speed_controller(motion.get_base_speed()));
    //       main_kappa = motion.mean_kappa();
    //     }
    //     else {
    //       if(distance_lock){
    //         motion.stop();
    //         sign_distance = vision.get_distance_from_sign();
    //         vTaskDelay(3000);
    //         motor.start_encode();
    //         distance_lock = false;
    //       }
    //       else{
    //         if(motor.get_distance_covered_mm() < sign_distance + (main_kappa * 3600)){
    //           motion.operate(motion.stanley(), motion.obstacle_controller(motion.get_base_speed()));
    //         }
    //         else{
    //           sign_distance = 0;
    //           vision.set_sign(vision.TRAVERSE);
    //           distance_lock = true;
    //         }
    //       }
    //     }
    //     uart.debug_println("3");
    //     break;
    //   }

    //   case vision.LEFT:
    //   {
    //     intersection.instruction();
    //     // uart.debug_println("4");
    //     break;
    //   }
      
    //   case vision.NOPASSING:
    //   {
    //     vision.set_sign(vision.TRAVERSE);
    //     uart.debug_println("5");
    //     break;
    //   }

    //   case vision.NOPASSINGEND:
    //   {
    //     vision.set_sign(vision.TRAVERSE);
    //     uart.debug_println("6");
    //     break;
    //   }

    //   case vision.PARK:
    //   {
    //     vision.set_sign(vision.TRAVERSE);
    //     uart.debug_println("7");
    //     break;
    //   }

    //   case vision.RIGHT:
    //   {
    //     intersection.instruction();
    //     // uart.debug_println("8");
    //     break;
    //   }

    //   case vision.STOP:
    //   {
    //     stop_detected = true;
    //     if(vision.get_distance_from_sign() < 420)
    //     {
    //       motion.stop();
    //       mapping.transmit_data();
    //       vTaskDelay(100);
    //       if(motion.min_of_front_VLs() < 500){
    //         stop_VL_flag = true;
    //       }
    //     }
    //     else 
    //     {
    //       motion.operate(motion.stanley(), motion.obstacle_controller(30)); 
    //     }
    //     break;
    //   }

    //   case vision.STRAIGHT:
    //   {
    //     intersection.instruction();
    //     // uart.debug_println("10");
        
    //     break;
    //   }

    //   case vision.TRAFFICLIGHT_GREEN:
    //   {
    //     vision.set_sign(vision.TRAVERSE);
    //     uart.debug_println("11");
    //     break;
    //   }

    //   case vision.TRAFFICLIGHT_RED:
    //   {
    //     vision.set_sign(vision.TRAVERSE);
    //     uart.debug_println("12");
    //     break;
    //   }

    //   case vision.TRAFFICLIGHT_YELLOW:
    //   {
    //     vision.set_sign(vision.TRAVERSE);
    //     uart.debug_println("13");
    //     break;
    //   }

    //   case vision.TUNNEL:
    //   {
    //     motion.operate(motion.stanley(), motion.speed_controller(30));
    //     uart.debug_println("14");
    //     break;
    //   }

    //   case vision.TUNNELEND:
    //   {
    //     // motion.operate(motion.stanley(), motion.speed_controller(motion.get_base_speed()));
    //     // vision.set_sign(vision.TRAVERSE);
        
    //     if(vision.get_distance_from_sign() > 400){
    //       motion.operate(motion.stanley(), motion.speed_controller(motion.get_base_speed())); 
    //     }
    //     else {
    //       if(distance_lock){
    //         // motion.stop();
    //         sign_distance = vision.get_distance_from_sign();
    //         // vTaskDelay(500);
    //         motor.start_encode();
    //         distance_lock = false;
    //       }
    //       else{
    //         if(motor.get_distance_covered_mm() < sign_distance + 100){
    //           motion.operate(motion.stanley(), motion.obstacle_controller(motion.get_base_speed()));
    //         }
    //         else{
    //           sign_distance = 0;
    //           vision.set_sign(vision.TRAVERSE);
    //           distance_lock = true;
    //         }
    //       }
    //     }
    //     uart.debug_println("15");
    //     break;
    //   }

    //   case vision.UPHILL:
    //   {
    //     vision.set_sign(vision.TRAVERSE);
    //     uart.debug_println("16");
    //     break;
    //   }

    //   case vision.PARK_LEFT:
    //   {
    //     Cross.instruction();
    //     // uart.debug_println("17");
    //     break;
    //   }

    //   case vision.PARK_RIGHT:
    //   {
    //     Parallel.instruction();
    //     // uart.debug_println("18");
    //     break;
    //   }

    //   case vision.TRAVERSE:
    //   {
        
    //     // if(vl.get_data(vl.RIGHT) < 500 + 100 + (1.5 * vision.get_distance_from_midline())){
    //     //   vl_counter += 1;
    //     // }
    //     // if(vl_counter > 80){
    //     //   motion.stop();
    //     //   vTaskDelay(500);
    //     //   vision.set_sign(vision.PARK_LEFT);
    //     //   vl_counter = 0;
    //     //   motor.start_encode();
    //     // }
    //     motion.operate(motion.stanley(), motion.obstacle_controller(50)); //edit: race:100  city:50
    //     break;
    //   }

    //   case vision.PARK_RIGHT_EXIT:
    //   {
    //     Parallel.instruction();
    //     uart.debug_println("20");
    //     break;
    //   }
      
    //   case vision.TEST:
    //   {
    //     //VL testl
    //     // vl.start_vl(VFR | VFMR | VF | VFML | VFL | VL | VBL | VBML | VB | VBMR | VBR | VR);
    //     // uart.debug_println("Front_VLs: ");
    //     // uart.debug_println(vl.get_data(vl.FRONT_LEFT));
    //     // uart.debug_println(vl.get_data(vl.FRONT_MID_LEFT));
    //     // uart.debug_println(vl.get_data(vl.FRONT));
    //     // uart.debug_println(vl.get_data(vl.FRONT_MID_RIGHT));
    //     // uart.debug_println(vl.get_data(vl.FRONT_RIGHT));
    //     // uart.debug_println("Right_VL: ");
    //     // uart.debug_println(vl.get_data(vl.RIGHT));
    //     // uart.debug_println("Back_VLs: ");
    //     // uart.debug_println(vl.get_data(vl.BACK_RIGHT));
    //     // uart.debug_println(vl.get_data(vl.BACK_MID_RIGHT));
    //     // uart.debug_println(vl.get_data(vl.BACK));
    //     // uart.debug_println(vl.get_data(vl.BACK_MID_LEFT));
    //     // uart.debug_println(vl.get_data(vl.BACK_LEFT));
    //     // uart.debug_println("Left_VL: ");
    //     // uart.debug_println(vl.get_data(vl.LEFT));
    //     // vTaskDelay(500);

    //     // //CMPS test
    //     // uart.debug_println("CMPS Data: ");
    //     // uart.debug_println(cmps.get_car_angle());

    //     // //Motor test
    //     // uart.debug_println("Motor : ");
    //     // motion.operate(0, 60);
    //     // uart.debug_println(motor.get_current_speed());

    //     // //Servo test
    //     // uart.debug_println("Servo : ");
    //     // motion.operate(70, 0);
    //     // vTaskDelay(1000);
    //     // motion.operate(-70, 0);
    //     // vTaskDelay(1000);

    //     // //Speed Controller test
    //     // uart.debug_println("Speed Controller: ");
    //     // motion.operate(0, motion.speed_controller(50));

    //     // //Sign test
    //     // uart.debug_print("Sign Class: ");
    //     // uart.debug_println(vision.get_sign());
    //     // uart.debug_print("Sign Distance: ");
    //     // uart.debug_println(vision.get_distance_from_sign());
        
    //     // //Line test
    //     // uart.debug_print("Line Distance: ");
    //     // uart.debug_println(vision.get_distance_from_midline());
    //     // uart.debug_print("Line Angle: ");
    //     // uart.debug_println(vision.get_angle());

    //     // //Crosswalk test
    //     // uart.debug_print("Crosswalk Angle: ");
    //     // uart.debug_println(vision.get_crosswalk_angle());

    //     // //TrafficLight test
    //     // uart.debug_print("TrafficLight Class: ");
    //     // uart.debug_println(vision.get_traffic_light());
    //   }
      
    //   default:
    //   {
    //     vision.set_sign(vision.TRAVERSE) ; 
    //     vision.set_distance_from_sign(0) ;                                                                                                                                     
    //   }
    // }
    
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

void loop()
{
  vTaskDelete(0);
}
