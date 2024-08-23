#include "Modules.h"
#include "Actuator.h"
#include "Motion.h"
#include "Mapping.h"
#include <WiFi.h>
#include <WebServer.h>

Mapping mapping;

WebServer server(80);

const char* ssid = "Galaxy S21 FE 5G 7c0b";     
const char* password = "vmpp2575"; 

void Mapping::mapping(){
    if(vl.get_data(vl.FRONT) <= sensitivity){
        x_real = (motion.x_global + (335 * cos((motion.theta_global + 0) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT) * cos((motion.theta_global + 0) * DEG_TO_RAD));
        y_real = (motion.y_global + (335 * sin((motion.theta_global + 0) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT) * sin((motion.theta_global + 0) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
          // uart.debug_print(x_grid);
          // uart.debug_print("\t");
          // uart.debug_println(y_grid);
        }
    }

    if(vl.get_data(vl.FRONT_MID_RIGHT) <= sensitivity){
        x_real = (motion.x_global + (339 * cos((motion.theta_global - 8.48) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT_MID_RIGHT) * cos((motion.theta_global + 0) * DEG_TO_RAD));
        y_real = (motion.y_global + (339 * sin((motion.theta_global - 8.48) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT_MID_RIGHT) * sin((motion.theta_global + 0) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.FRONT_RIGHT) <= sensitivity){
        x_real = (motion.x_global + (359 * cos((motion.theta_global - 14.03) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT_RIGHT) * cos((motion.theta_global - 45) * DEG_TO_RAD));
        y_real = (motion.y_global + (359 * sin((motion.theta_global - 14.03) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT_RIGHT) * sin((motion.theta_global - 45) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.RIGHT) <= sensitivity){
        x_real = (motion.x_global + (158 * cos((motion.theta_global - 34.69) * DEG_TO_RAD))) + (vl.get_data(vl.RIGHT) * cos((motion.theta_global - 90) * DEG_TO_RAD));
        y_real = (motion.y_global + (158 * sin((motion.theta_global - 34.69) * DEG_TO_RAD))) + (vl.get_data(vl.RIGHT) * sin((motion.theta_global - 90) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.BACK_RIGHT) <= sensitivity){
        x_real = (motion.x_global + (94 * cos((motion.theta_global - 122.01) * DEG_TO_RAD))) + (vl.get_data(vl.BACK_RIGHT) * cos((motion.theta_global - 135) * DEG_TO_RAD));
        y_real = (motion.y_global + (94 * sin((motion.theta_global - 122.01) * DEG_TO_RAD))) + (vl.get_data(vl.BACK_RIGHT) * sin((motion.theta_global - 135) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.BACK_MID_RIGHT) <= sensitivity){
        x_real = (motion.x_global + (82 * cos((motion.theta_global - 142.43) * DEG_TO_RAD))) + (vl.get_data(vl.BACK_MID_RIGHT) * cos((motion.theta_global + 180) * DEG_TO_RAD));
        y_real = (motion.y_global + (82 * sin((motion.theta_global - 142.43) * DEG_TO_RAD))) + (vl.get_data(vl.BACK_MID_RIGHT) * sin((motion.theta_global + 180) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.BACK) <= sensitivity){
        x_real = (motion.x_global + (65 * cos((motion.theta_global + 180) * DEG_TO_RAD))) + (vl.get_data(vl.BACK) * cos((motion.theta_global + 180) * DEG_TO_RAD));
        y_real = (motion.y_global + (65 * sin((motion.theta_global + 180) * DEG_TO_RAD))) + (vl.get_data(vl.BACK) * sin((motion.theta_global + 180) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.BACK_MID_LEFT) <= sensitivity){
        x_real = (motion.x_global + (82 * cos((motion.theta_global + 142.43) * DEG_TO_RAD))) + (vl.get_data(vl.BACK_MID_LEFT) * cos((motion.theta_global - 180) * DEG_TO_RAD));
        y_real = (motion.y_global + (82 * sin((motion.theta_global + 142.43) * DEG_TO_RAD))) + (vl.get_data(vl.BACK_MID_LEFT) * sin((motion.theta_global - 180) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);
        
        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.BACK_LEFT) <= sensitivity){
        x_real = (motion.x_global + (94 * cos((motion.theta_global + 122.01) * DEG_TO_RAD))) + (vl.get_data(vl.BACK_LEFT) * cos((motion.theta_global + 135) * DEG_TO_RAD));
        y_real = (motion.y_global + (94 * sin((motion.theta_global + 122.01) * DEG_TO_RAD))) + (vl.get_data(vl.BACK_LEFT) * sin((motion.theta_global + 135) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.LEFT) <= sensitivity){
        x_real = (motion.x_global + (158 * cos((motion.theta_global + 34.69) * DEG_TO_RAD))) + (vl.get_data(vl.LEFT) * cos((motion.theta_global + 90) * DEG_TO_RAD));
        y_real = (motion.y_global + (158 * sin((motion.theta_global + 34.69) * DEG_TO_RAD))) + (vl.get_data(vl.LEFT) * sin((motion.theta_global + 90) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.FRONT_LEFT) <= sensitivity){
        x_real = (motion.x_global + (359 * cos((motion.theta_global + 14.03) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT_LEFT) * cos((motion.theta_global + 45) * DEG_TO_RAD));
        y_real = (motion.y_global + (359 * sin((motion.theta_global + 14.03) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT_LEFT) * sin((motion.theta_global + 45) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        // uart.debug_print("theta: ");
        // uart.debug_print(motion.theta_global);
        // uart.debug_print("\tx: ");
        // uart.debug_print(x_real);
        // uart.debug_print("\ty:  ");
        // uart.debug_println(y_real);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
    if(vl.get_data(vl.FRONT_MID_LEFT) <= sensitivity){
        x_real = (motion.x_global + (339 * cos((motion.theta_global + 8.48) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT_MID_LEFT) * cos((motion.theta_global - 0) * DEG_TO_RAD));
        y_real = (motion.y_global + (339 * sin((motion.theta_global + 8.48) * DEG_TO_RAD))) + (vl.get_data(vl.FRONT_MID_LEFT) * sin((motion.theta_global - 0) * DEG_TO_RAD));
        x_grid = (x_real / 500);
        y_grid = (y_real / 500);

        if(x_grid >= 0 && x_grid < 18 && y_grid >= 0 && y_grid < 8 ){
          if(grid[x_grid][y_grid] < 255){
            grid[x_grid][y_grid] += 1;
          }
        }
    }
}

void handleRoota() {
  String send_data = "";
  for(int j = 0; j < 18; j++){
    for(int i = 0; i < 8; i++){
      send_data += mapping.grid[j][i];
      send_data += "\t"; 
    }
    send_data += "\n";
  }
  server.send(200, "text/plain", send_data); // داده‌ای که می‌خواهید ارسال کنید
}
void handleRootb() {
  String send_data = "";
  for(int j = 50; j < 100; j++){
    for(int i = 0; i < 225; i++){
      send_data += mapping.grid[j][i];
      send_data += "\t"; 
    }
    send_data += "\n";
  }
  server.send(200, "text/plain", send_data); // داده‌ای که می‌خواهید ارسال کنید
}


void Mapping::begin_server(){
    // Serial.begin(115200);
    WiFi.begin(ssid, password);

    // while (WiFi.status() != WL_CONNECTED) {
    //     // delay(1000);
    //     // Serial.println("Connecting to WiFi...");
    // }
    
    // Serial.println("Connected to WiFi");
    // Serial.print("IP Address: ");
    // Serial.println(WiFi.localIP());
    server.on("/first", handleRoota);
    server.on("/second", handleRootb);
    server.begin();
}

void Mapping::client(){
    server.handleClient();
}
