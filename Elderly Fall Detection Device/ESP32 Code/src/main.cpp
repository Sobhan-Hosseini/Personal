/*
  Fall Detection Device
  Authors: Seyed Sobhan Hosseini - Ali Saemi
  Date: Monday, August 5, 2024, 11:05:59 PM
*/

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <cmath>
//#include "driver/touch_pad.h"
//#include "driver/rtc_io.h"  //RTC clock
//#include "driver/dac.h" //DAC channel
//#include "driver/can.h" //CAN driver
//#include "driver/ledc.h"  //PWM functions
//#include "driver/i2c.h" //I2C driver
//#include "driver/spi_master.h"  //spi_master
//#include "driver/uart.h"  //uart header
//#include "esp_bt.h" //BLE header
//#include "driver/uart.h"  //uart header
//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>
//#include <freertos/semphr.h>
//#include <esp_sleep.h>

#define gamma 1.0700706614911336e-05
#define bias -2.72489653

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

//**********sensors setup**********
void updateSerial(){
  while(Serial.available()){
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(Serial2.available()){
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}

void test_sim800_module(){
  Serial2.println("AT");
  updateSerial();
  Serial.println();
  Serial2.println("AT+CSQ");
  updateSerial();
  Serial2.println("AT+CCID");
  updateSerial();
  Serial2.println("AT+CREG?");
  updateSerial();
  Serial2.println("ATI");
  updateSerial();
  Serial2.println("AT+CBC");
  updateSerial();
}

void send_SMS(){
  Serial2.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  Serial2.println("AT+CMGS=\"+989216503788\"");
  updateSerial();
  Serial2.print("Enipha !"); //text content
  updateSerial();
  Serial.println();
  Serial.println("Message Sent");
  Serial2.write(26);
}

void mpu_test(){
  Serial.println("MPU6050 test!");

  if(!mpu.begin()){
    Serial.println("Failed to find MPU6050 module");
    while(true){
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

void power_saving(){
  //mpu6050
  mpu.setGyroStandby(true, true, true); //check
  mpu.setTemperatureStandby(true);  //check
  mpu.enableCycle(true);  //check
  mpu.setCycleRate(MPU6050_CYCLE_20_HZ); // <20HZ ours is 10 Hz //check

  //sim800L

  //ESP32
  setCpuFrequencyMhz(160); //check //160 => great  //80 => not bad
  // touch_pad_deinit(); 
  // rtc_gpio_deinit(GPIO_NUM_12);  //turn off the gpio12
  // dac_output_disable(DAC_CHANNEL_1); //turn off the DAC channel 1
  // can_driver_uninstall();  //unistall the can driver
  // ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
  // ledc_fade_func_uninstall();  // Disable LEDC (PWM) channel
  // i2c_driver_delete(I2C_NUM_0);  // you can delete I2C master or slave mode
  // spi_bus_free(HSPI_HOST);
  // uart_driver_delete(UART_NUM_1);
  // esp_bt_controller_disable();
  // esp_bt_controller_deinit();
  // esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);  // Disable Bluetooth
  // uart_driver_delete(UART_NUM_1);
}
void setting(){
  power_saving();
  mpu_test();
  // test_sim800_module();
}
//**********coefficients**********
double w[18] = {
  -5.29982173e-02, -1.47561951e-03, -1.31400395e-02, -7.95977617e-04,
  -3.68128973e-04, -6.69803020e-03, -5.35898165e-03, -1.43389589e-03,
   6.45254778e-04,  1.05534030e-02,  5.84856685e-03,  7.07064662e-05,
   2.76475047e-06,  1.79971194e-03,  4.07638113e-04,  9.87482981e-03,
   1.03544516e-03,  5.20305698e-02}; 

double support_vectors[18][48] = {
{-9.08935547e-01,0.00000000e+00,-4.83235407e-01,2.64394295e-01
,1.35396242e-01,-1.73661598e+00,4.93408203e-01,-2.90231201e+00
,-8.97460938e-01,9.57031250e-02,-5.31307588e-01,2.31332049e-01
,3.81853369e-01,-7.13135129e-01,3.92822266e-01,-3.19481934e+00
,-6.69433594e-01,0.00000000e+00,-5.16166814e-01,1.25866768e-01
,1.64929930e+00,2.33753584e+00,1.08154297e-01,-3.10222412e+00
,-2.23388672e-02,1.78222656e-01,2.53954916e-03,1.46643997e-02
,7.09157564e+00,7.19713662e+01,2.31933594e-03,1.35302734e-02
,-6.43310547e-02,7.58056641e-01,2.46898360e-03,5.05939359e-02
,1.24107603e+01,1.72515418e+02,2.19726562e-03,7.30468750e-03
,-4.41894531e-02,6.10107422e-01,2.70825763e-03,4.02418365e-02
,1.28420169e+01,1.80410170e+02,4.27246094e-03,1.02001953e-02},
{-8.89160156e-01,7.95410156e-01,1.97779363e-02,7.54825817e-01
,-2.93701992e-01,-1.86557604e+00,1.56225586e+00,1.27927923e-01
,0.00000000e+00,1.22949219e+00,9.27025301e-01,1.12454940e-01
,-3.11627576e+00,2.40351488e+01,1.11328125e-01,5.57210833e+00
,-7.13623047e-01,4.36869464e-02,-5.23387356e-01,2.03009278e-01
,4.95235590e-01,-1.32767080e+00,4.01123047e-01,-3.14792323e+00
,-6.80419922e-01,9.70178319e-02,1.81037168e-03,4.78774209e-02
,-1.05945560e+01,1.43006021e+02,1.46484375e-03,1.77294922e-02
,-9.76562500e-01,4.26838840e-01,-4.47766884e-03,7.74713686e-02
,-6.87091842e+00,9.31544885e+01,2.19726562e-03,-1.71679687e-02
,-2.96513787e-01,6.94091797e-01,2.10642260e-03,5.31304606e-02
,7.51293210e+00,1.05531290e+02,3.54003906e-03,5.73730469e-03},
{-1.00439453e+00,0.00000000e+00,-5.55685962e-01,3.15344129e-01
,1.63710363e-01,-1.85217434e+00,6.23291016e-01,-3.33697510e+00
,-9.32861328e-01,2.69042969e-01,-3.55100511e-01,2.54640213e-01
,2.01739101e-01,-6.96696555e-01,3.99902344e-01,-2.13590576e+00
,-9.43115234e-01,0.00000000e+00,-6.41874092e-01,2.32886227e-01
,2.46560318e-01,-1.03086707e+00,4.01855469e-01,-3.85926025e+00
,-2.96630859e-02,2.01660156e-01,3.07771296e-03,1.71154848e-02
,6.72884998e+00,6.42119515e+01,2.44140625e-03,1.65087891e-02
,-5.29785156e-02,5.73242188e-01,1.55000908e-03,4.05909562e-02
,1.04187578e+01,1.36070874e+02,2.19726562e-03,3.59863281e-03
,-1.15356445e-01,8.97705078e-01,3.08420175e-03,5.97039675e-02
,1.24633476e+01,1.74824865e+02,2.92968750e-03,9.64355469e-03},
{-6.80908203e-01,0.00000000e+00,-6.69058055e-01,3.88639271e-02
,1.69485232e+01,2.88811739e+02,5.37109375e-03,-4.02097168e+00
,-4.04541016e-01,0.00000000e+00,-3.92771978e-01,2.38362906e-02
,1.48704190e+01,2.41946758e+02,1.34277344e-02,-2.36046387e+00
,0.00000000e+00,1.39746094e+00,1.37969545e+00,7.97804373e-02
,-1.71820357e+01,2.94153561e+02,5.61523438e-03,8.29197754e+00
,-2.56347656e-03,6.68701172e-01,3.35551417e-03,4.29421502e-02
,1.38613705e+01,2.00019186e+02,1.34277344e-03,1.35156250e-02
,-3.54003906e-03,3.88671875e-01,1.98638004e-03,2.49983035e-02
,1.38088325e+01,1.98850996e+02,2.19726562e-03,8.04687500e-03
,-1.38256836e+00,5.24902344e-03,-6.87365358e-03,8.88852884e-02
,-1.38422009e+01,1.99405118e+02,2.68554688e-03,-2.75781250e-02},
{-6.98242188e-01,0.00000000e+00,-6.88298932e-01,3.98214371e-02
,1.71549364e+01,2.93533935e+02,2.92968750e-03,-4.13666504e+00
,-3.68652344e-02,5.12695312e-03,-1.37708290e-02,1.23327574e-02
,-4.94979897e-01,-1.34513258e+00,2.41699219e-02,-8.29296875e-02
,0.00000000e+00,1.31835938e+00,1.30714432e+00,7.55418954e-02
,-1.72118531e+01,2.94835342e+02,5.12695312e-03,7.85583984e+00
,-2.56347656e-03,6.88232422e-01,3.42972968e-03,4.42593653e-02
,1.38369560e+01,1.99235075e+02,3.66210938e-04,1.37890625e-02
,-3.41796875e-03,3.24707031e-02,4.78548069e-05,2.33071400e-03
,9.99949218e+00,1.29296258e+02,1.09863281e-03,-5.85937500e-05
,-1.31250000e+00,4.02832031e-03,-6.55529745e-03,8.43125931e-02
,-1.38571505e+01,1.99859163e+02,2.56347656e-03,-2.63378906e-02},
{-1.02197266e+00,0.00000000e+00,-6.86452872e-01,1.89689524e-01
,3.50901841e-01,-7.20918992e-01,3.31542969e-01,-4.12853516e+00
,-1.54541016e-01,1.35302734e+00,5.05887763e-01,2.96705322e-01
,3.67900211e-01,7.29693176e-02,4.05273438e-01,3.04223145e+00
,0.00000000e+00,1.51660156e+00,8.04927585e-01,2.56976402e-01
,1.91646962e-01,2.57012828e-01,3.52294922e-01,4.83511719e+00
,-7.71484375e-02,7.29980469e-01,2.51237736e-03,5.06470324e-02
,1.09919467e+01,1.47138728e+02,1.64794922e-02,7.82226562e-03
,-4.71191406e-01,1.62963867e-01,-1.85376544e-03,5.85767185e-02
,-1.30675612e+00,1.34731510e+01,5.92041016e-02,-6.42578125e-03
,-9.35791016e-01,1.01318359e-01,-5.06247080e-03,7.31205715e-02
,-7.78513151e+00,9.01022060e+01,4.26025391e-02,-2.10937500e-02},
{-6.81884766e-01,0.00000000e+00,-6.21235692e-01,5.37718372e-02
,5.11197957e+00,5.66689405e+01,7.29980469e-02,-3.73324707e+00
,0.00000000e+00,1.52392578e+00,1.40378588e+00,1.20892697e-01
,-5.11336699e+00,5.77368868e+01,1.84082031e-01,8.43770020e+00
,-2.49267578e-01,0.00000000e+00,-1.70559398e-01,3.93914387e-02
,2.21118891e-01,-6.29743544e-01,7.34863281e-02,-1.02516357e+00
,-8.54492188e-03,5.60546875e-01,3.11705124e-03,3.61338366e-02
,1.37157812e+01,1.96868576e+02,1.34277344e-03,1.31835938e-02
,-1.52172852e+00,1.85546875e-02,-6.87284248e-03,9.78633951e-02
,-1.38408197e+01,1.99444445e+02,1.22070312e-03,-2.61816406e-02
,-1.25732422e-02,2.05078125e-01,8.69091959e-04,1.34948761e-02
,1.29125475e+01,1.81869705e+02,2.80761719e-03,3.20800781e-03},
{-8.29101562e-01,0.00000000e+00,-6.91926975e-01,1.60803210e-01
,6.07224448e-01,-8.02733554e-01,3.26660156e-01,-4.15718018e+00
,0.00000000e+00,1.50415039e+00,1.32279365e+00,1.48561444e-01
,-2.03272582e+00,1.85702184e+01,2.63427734e-01,7.95094971e+00
,-7.24365234e-01,0.00000000e+00,-4.94813837e-01,1.69923350e-01
,-3.23821895e-01,-1.62400065e+00,3.43261719e-01,-2.97524414e+00
,-2.56347656e-03,4.97314453e-01,3.56153317e-03,3.20993235e-02
,1.35721139e+01,1.94283855e+02,2.31933594e-03,1.64404297e-02
,-1.47656250e+00,1.98974609e-02,-6.53258669e-03,9.49909626e-02
,-1.38308473e+01,1.99249275e+02,2.19726562e-03,-2.45361328e-02
,-5.15136719e-02,7.18505859e-01,2.37246288e-03,4.66156891e-02
,1.35038250e+01,1.93302734e+02,2.80761719e-03,7.07031250e-03},
{-8.59130859e-01,1.00000000e+00,-8.52954345e-01,1.06980369e-01
,1.72627730e+01,2.96003333e+02,0.00000000e+00,-5.13619385e+00
,-4.83154297e-01,1.00000000e+00,-4.78226874e-01,8.53454684e-02
,1.72627730e+01,2.96003333e+02,0.00000000e+00,-2.88409424e+00
,1.30371094e-01,1.00000000e+00,1.33260226e-01,5.00412442e-02
,1.72627730e+01,2.96003333e+02,0.00000000e+00,7.90922852e-01
,0.00000000e+00,1.85913086e+00,9.26477172e-03,1.19448105e-01
,1.38590216e+01,1.99814205e+02,0.00000000e+00,3.71826172e-02
,0.00000000e+00,1.48315430e+00,7.39113437e-03,9.52918240e-02
,1.38590216e+01,1.99814205e+02,0.00000000e+00,2.96630859e-02
,0.00000000e+00,8.69628906e-01,4.33369887e-03,5.58731649e-02
,1.38590216e+01,1.99814205e+02,0.00000000e+00,1.73925781e-02},
{-8.56689453e-01,1.00000000e+00,-4.15581687e-01,3.74687826e-01
,2.20172576e-01,-1.39677288e+00,7.32910156e-01,-2.50415283e+00
,-1.05639648e+00,1.00000000e+00,-7.38491324e-01,3.16655411e-01
,4.86680457e-01,8.76909800e-01,6.00585938e-01,-4.45126709e+00
,-6.81884766e-01,1.00000000e+00,-2.31531289e-01,2.68262970e-01
,2.80666347e-01,-5.68636259e-01,5.29785156e-01,-1.39882324e+00
,-3.97949219e-02,1.03198242e+00,7.57403706e-03,6.93097487e-02
,1.22200019e+01,1.65690979e+02,2.19726562e-03,3.52978516e-02
,-1.09130859e-01,2.05200195e+00,8.20628828e-03,1.32716377e-01
,1.36241616e+01,1.95343157e+02,1.09863281e-03,2.89013672e-02
,-5.93261719e-02,9.51171875e-01,6.56219178e-03,6.23511213e-02
,1.29809021e+01,1.82787391e+02,3.90625000e-03,2.99902344e-02},
{-1.05761719e+00,1.00000000e+00,-6.47559892e-01,3.25060631e-01
,1.31320950e+00,1.00076830e+00,6.50634766e-01,-3.90003418e+00
,-1.10888672e+00,1.00000000e+00,-4.64610154e-01,3.34590617e-01
,-6.87904487e-01,2.43599792e-01,5.72509766e-01,-2.80397461e+00
,-6.97509766e-01,1.00000000e+00,-2.82395271e-01,5.72840825e-01
,1.08273145e+00,-6.91035517e-01,1.14477539e+00,-1.70369629e+00
,-3.91845703e-02,1.12036133e+00,7.93740916e-03,7.38870965e-02
,1.27659693e+01,1.77996696e+02,2.44140625e-03,3.65527344e-02
,-1.80786133e-01,1.88403320e+00,7.44507241e-03,1.23296723e-01
,1.31061915e+01,1.86142359e+02,3.41796875e-03,2.59570312e-02
,-2.96630859e-02,1.98974609e-01,5.74419902e-03,2.24258389e-02
,4.76036133e+00,2.77799213e+01,4.39453125e-03,3.26464844e-02},
{-8.59130859e-01,1.00000000e+00,-8.52954345e-01,1.06980369e-01
,1.72627730e+01,2.96003333e+02,0.00000000e+00,-5.13619385e+00
,-4.83154297e-01,1.00000000e+00,-4.78226874e-01,8.53454684e-02
,1.72627730e+01,2.96003333e+02,0.00000000e+00,-2.88409424e+00
,1.30371094e-01,1.00000000e+00,1.33260226e-01,5.00412442e-02
,1.72627730e+01,2.96003333e+02,0.00000000e+00,7.90922852e-01
,0.00000000e+00,1.85913086e+00,9.26477172e-03,1.19448105e-01
,1.38590216e+01,1.99814205e+02,0.00000000e+00,3.71826172e-02
,0.00000000e+00,1.48315430e+00,7.39113437e-03,9.52918240e-02
,1.38590216e+01,1.99814205e+02,0.00000000e+00,2.96630859e-02
,0.00000000e+00,8.69628906e-01,4.33369887e-03,5.58731649e-02
,1.38590216e+01,1.99814205e+02,0.00000000e+00,1.73925781e-02},
{-8.59130859e-01,1.00000000e+00,-8.52954345e-01,1.06980369e-01
,1.72627730e+01,2.96003333e+02,0.00000000e+00,-5.13619385e+00
,-4.83154297e-01,1.00000000e+00,-4.78226874e-01,8.53454684e-02
,1.72627730e+01,2.96003333e+02,0.00000000e+00,-2.88409424e+00
,1.30371094e-01,1.00000000e+00,1.33260226e-01,5.00412442e-02
,1.72627730e+01,2.96003333e+02,0.00000000e+00,7.90922852e-01
,0.00000000e+00,1.85913086e+00,9.26477172e-03,1.19448105e-01
,1.38590216e+01,1.99814205e+02,0.00000000e+00,3.71826172e-02
,0.00000000e+00,1.48315430e+00,7.39113437e-03,9.52918240e-02
,1.38590216e+01,1.99814205e+02,0.00000000e+00,2.96630859e-02
,0.00000000e+00,8.69628906e-01,4.33369887e-03,5.58731649e-02
,1.38590216e+01,1.99814205e+02,0.00000000e+00,1.73925781e-02},
{-8.66455078e-01,1.08374023e+00,-1.92722663e-01,7.07187407e-01
,2.11778941e-01,-1.79813723e+00,1.36108398e+00,-1.16161865e+00
,1.41113281e-01,1.00000000e+00,4.74769972e-01,1.36997031e-01
,-2.96011070e-02,-2.47268875e-01,2.24853516e-01,2.84243164e+00
,-8.93066406e-01,1.00000000e+00,-3.31955007e-01,4.66252615e-01
,-9.31899568e-02,-1.68960346e+00,9.94384766e-01,-2.00969971e+00
,-8.77685547e-02,4.97802734e-01,6.99734609e-03,4.40171879e-02
,6.38660096e+00,5.73037638e+01,1.46484375e-03,3.71435547e-02
,-8.41064453e-02,6.58691406e-01,2.52778823e-03,4.48835951e-02
,1.16063572e+01,1.58314311e+02,2.19726562e-03,8.63281250e-03
,-1.10473633e-01,1.86450195e+00,5.96887329e-03,1.21939267e-01
,1.31966983e+01,1.87297945e+02,3.90625000e-03,1.73388672e-02},
{-8.39599609e-01,1.00000000e+00,-7.99262874e-01,1.04706552e-01
,1.68575478e+01,2.86680075e+02,1.39160156e-02,-4.81343750e+00
,-3.55712891e-01,1.00000000e+00,-2.40604236e-01,1.12356011e-01
,4.57485037e+00,4.71330987e+01,1.48681641e-01,-1.45520752e+00
,-7.21435547e-01,1.00000000e+00,-5.83403629e-01,1.16238102e-01
,8.34543877e+00,1.11574489e+02,1.34765625e-01,-3.51687500e+00
,-2.81982422e-02,1.79785156e+00,9.00805907e-03,1.15558990e-01
,1.38403180e+01,1.99453862e+02,2.31933594e-03,3.62500000e-02
,-3.64990234e-02,1.18090820e+00,6.35333394e-03,7.62937897e-02
,1.36158253e+01,1.95043104e+02,2.07519531e-03,2.64599609e-02
,-4.83398438e-02,1.64794922e+00,7.78816705e-03,1.06107559e-01
,1.37950413e+01,1.98446078e+02,2.68554688e-03,3.04296875e-02},
{-6.88232422e-01,1.00000000e+00,-2.49492651e-01,4.24532482e-01
,9.48404839e-02,-1.69172467e+00,8.36669922e-01,-1.50508785e+00
,1.37451172e-01,1.01708984e+00,5.88284410e-01,4.13921322e-01
,-3.95667446e-02,-1.94045631e+00,8.62792969e-01,3.52998533e+00
,-1.80906281e-02,1.00000000e+00,5.04340474e-01,2.49399160e-01
,-8.07197523e-02,-1.59840198e+00,4.80957031e-01,3.01851979e+00
,-2.06779324e-01,8.46435547e-01,7.00667372e-03,6.23632177e-02
,9.18517727e+00,1.14501855e+02,1.34277344e-03,3.37158203e-02
,-3.44429172e-01,3.40259750e-01,2.81329488e-03,4.09096845e-02
,-2.08268748e-01,6.16780149e+01,2.19726562e-03,1.70263672e-02
,-1.29111078e-01,7.19726562e-01,1.98962443e-03,4.97318277e-02
,1.12626505e+01,1.51013475e+02,3.90625000e-03,4.78027344e-03},
{-8.59130859e-01,1.00000000e+00,-8.52954345e-01,1.06980369e-01
,1.72627730e+01,2.96003333e+02,0.00000000e+00,-5.13619385e+00
,-4.83154297e-01,1.00000000e+00,-4.78226874e-01,8.53454684e-02
,1.72627730e+01,2.96003333e+02,0.00000000e+00,-2.88409424e+00
,1.30371094e-01,1.00000000e+00,1.33260226e-01,5.00412442e-02
,1.72627730e+01,2.96003333e+02,0.00000000e+00,7.90922852e-01
,0.00000000e+00,1.85913086e+00,9.26477172e-03,1.19448105e-01
,1.38590216e+01,1.99814205e+02,0.00000000e+00,3.71826172e-02
,0.00000000e+00,1.48315430e+00,7.39113437e-03,9.52918240e-02
,1.38590216e+01,1.99814205e+02,0.00000000e+00,2.96630859e-02
,0.00000000e+00,8.69628906e-01,4.33369887e-03,5.58731649e-02
,1.38590216e+01,1.99814205e+02,0.00000000e+00,1.73925781e-02},
{-8.66455078e-01,1.08374023e+00,-1.92722663e-01,7.07187407e-01
,2.11778941e-01,-1.79813723e+00,1.36108398e+00,-1.16161865e+00
,1.41113281e-01,1.00000000e+00,4.74769972e-01,1.36997031e-01
,-2.96011070e-02,-2.47268875e-01,2.24853516e-01,2.84243164e+00
,-8.93066406e-01,1.00000000e+00,-3.31955007e-01,4.66252615e-01
,-9.31899568e-02,-1.68960346e+00,9.94384766e-01,-2.00969971e+00
,-8.77685547e-02,4.97802734e-01,6.99734609e-03,4.40171879e-02
,6.38660096e+00,5.73037638e+01,1.46484375e-03,3.71435547e-02
,-8.41064453e-02,6.58691406e-01,2.52778823e-03,4.48835951e-02
,1.16063572e+01,1.58314311e+02,2.19726562e-03,8.63281250e-03
,-1.10473633e-01,1.86450195e+00,5.96887329e-03,1.21939267e-01
,1.31966983e+01,1.87297945e+02,3.90625000e-03,1.73388672e-02}
};

double transposed_sv[48][18];
double K[18];

//**********signal variables**********
const int signalSize = 300;
double x[signalSize];
double y[signalSize];
double z[signalSize];
double dx[signalSize];
double dy[signalSize];
double dz[signalSize];

const int featuresSize = 8;
double features_x[featuresSize];
double features_y[featuresSize];
double features_z[featuresSize];
double features_dx[featuresSize];
double features_dy[featuresSize];
double features_dz[featuresSize];

double input[48];

double norms[18];
double lin_values[18];
double distances[18][48];

int optime = 0;
int last_optime = 0;
int last_optime2 = 0;

float timeInterval = 0.01; //based on frequency of operation (10 milli seconds)

bool core0_flag = false;

double sorted[signalSize]; //supporting array for notsort function

//**********signal analysis functions**********
//^sort
void notSort(double signal[], int size){
  for(int k = 0; k < size - 1; k++){
    sorted[k] = signal[k];
  }
  for (int i = 0; i < size - 1; i++){
    for (int j = 0; j < size - 1 - i; j++){
      if (sorted[j] > sorted[j + 1]){
        double temp = sorted[j];
        sorted[j] = sorted[j + 1];
        sorted[j + 1] = temp;
      }
    }
  }


}

//^shift and insert
void shiftAndInsert(double signal[], int size, double newData){
    for(int i = size - 1; i > 0; i--){
        signal[i] = signal[i - 1];
    }

    signal[0] = newData;
}

//&min_ts
double min_ts(double signal[], int size){
	double min = signal[0];
    for(int i = 1; i < size; i++){
        if(signal[i] < min){
            min = signal[i];
        }
    }
    return min;
}

//&max_ts
double max_ts(double signal[], int size){
	double max = signal[0];
    for(int i = 1; i < size; i++){
        if(signal[i] > max){
            max = signal[i];
        }
    }
    return max;
}

//&mean_ts
double mean_ts(double signal[], int size){
    double sum = 0.0;
    for(int i = 0; i < size; i++){
        sum += signal[i];
    }
    return sum / size;
}

//&std_ts(standard deviation)
double std_ts(double signal[], int size){
	  double mean = mean_ts(signal, size);

    double sumSquaredDiff = 0.0;
    for(int i = 0; i < size; i++){
        sumSquaredDiff += pow(signal[i] - mean, 2);
    }

    double standardDeviation = sqrt(sumSquaredDiff / size);
    return standardDeviation;
}

//&skew_ts(skewness)
double skew_ts(double signal[], int size){
	  double mean = mean_ts(signal, size);
    
    double sumCubedDiff = 0.0;
    for(int i = 0; i < size; i++){
        sumCubedDiff += pow(signal[i] - mean, 3);
    }
    
    double variance = 0.0;
    for(int i = 0; i < size; i++){
        variance += pow(signal[i] - mean, 2);
    }
    variance /= size;
    
    double skewness = sumCubedDiff / (size * pow(variance, 1.5));	//size or (size - 1) ??
    return skewness;
}

//&kurtosis_ts
double kurtosis_ts(double signal[], int size){
    double mean = mean_ts(signal, size);
    
    double variance = 0.0;
    for(int i = 0; i < size; i++){
        variance += pow(signal[i] - mean, 2);
    }
    variance /= size;

    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += pow(signal[i] - mean, 4);
    }

    double kurtosis = (sum / size) / pow(variance, 2) - 3;
    return kurtosis;
}

//&iqr_ts(interquartile range)
double iqr_ts(double signal[], int size){
    notSort(signal, size);

    double Q1 = signal[size / 4]; // 25th percentile
    double Q3 = signal[(3 * size) / 4]; // 75th percentile

    return Q3 - Q1;
}

//&area_ts
double area_ts(double signal[], int size, double timeInterval){	//timeInterval : time between getting two datas
    double area = 0.0;

    for(int i = 1; i < size; i++){
        area += 0.5 * (signal[i - 1] + signal[i]) * timeInterval;	//trapezoidal rule
    }

    return area;
}

//&derivative
void derivative(double input[], double output[], int size, double deltaT){
    output[0] = (input[1] - input[0]) / deltaT;	//forward difference

    for(int i = 1; i < size - 1; i++){
        output[i] = (input[i+1] - input[i-1]) / (2 * deltaT);	//central difference
    }

    output[size - 1] = (input[size - 1] - input[size - 2]) / deltaT;	//backward difference
}

//%mergeFeatures
void mergeFeatures(double merged_array[], double min_ts, double max_ts, double mean_ts, double std_ts, double skew_ts, double kurtosis_ts, double iqr_ts, double area_ts){
    merged_array[0] = min_ts;
    merged_array[1] = max_ts;
    merged_array[2] = mean_ts;
    merged_array[3] = std_ts;
    merged_array[4] = skew_ts;
    merged_array[5] = kurtosis_ts;
    merged_array[6] = iqr_ts;
    merged_array[7] = area_ts;
}

//%merge_xyz
void merge_xyz(double merged_array[], double x[], double y[], double z[], double dx[], double dy[], double dz[]){
    for(int i = 0; i < 8; i++){
        merged_array[i] = x[i];
        merged_array[i + 8] = y[i];
        merged_array[i + 16] = z[i];
        merged_array[i + 24] = dx[i];
        merged_array[i + 32] = dy[i];
        merged_array[i + 40] = dz[i];
    }
}

//**********data processing functions**********
// //@norms(distances)
// void calculateDistances(double distances[18][48],double input[48], double support_vectors[18][48]){
//   for(int i = 0; i < 18; i++){
//         for(int j = 0; j < 48; j++){
//             distances[i][j] = input[j] - static_cast<double>(support_vectors[i][j]);
//         }
//     }
// }

// void calculateNorms(double distances[18][48], double norms[18]){
//   for(int i = 0; i < 18; i++){
//         double normSquared = 0.0;

//         for(int j = 0; j < 48; j++){
//             normSquared += distances[i][j] * distances[i][j];
//         }

//         norms[i] = sqrt(normSquared);
//     }
// }

// //@lin_values
// void calculateLin(double norms[], double lin_values[]){
//     for(int i = 0; i < 18; i++){
//         double Lin = -gamma * (norms[i] * norms[i]);
//         lin_values[i] = Lin;
//     }
// }

// //@prediction
// double calculatePrediction(double lin_values[], double dual_coef[]){
//     double prediction = 0.0;

//     for(int i = 0; i < 18; i++){
//         prediction += lin_values[i] * dual_coef[i];
//     }

//     prediction += bias;
//     return prediction;
// }

//new data analises without threshold
void transposeMatrix(double matrix[18][48], double transposed[48][18]) {
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 48; j++) {
            transposed[j][i] = matrix[i][j]; // Transpose
        }
    }
}

void dotProduct(double vector[48], double transposed[48][18], double result[18]) {
    for (int j = 0; j < 18; j++) {
        result[j] = 0; // Initialize the result
        for (int i = 0; i < 48; i++) {
            result[j] += vector[i] * transposed[i][j]; // Dot product calculation
        }
    }
}

double calculatePrediction(double w[18], double Kt[18]){
  double prediction = 0.0;

  for(int i = 0; i < 18; i++){
      prediction += w[i] * Kt[i];
  }

  prediction += bias;
  return prediction;
}

//**********SETUP**********
void setup(void) {
  Serial.begin(115200);
  Serial2.begin(115200);

  while(!Serial)
    delay(10);

  setting();

  Serial.println("");
  delay(100);
}

void loop() {
  mpu.getEvent(&a, &g, &temp);

  double newData_x = static_cast<double>(a.magnetic.x) / 10;
	double newData_y = static_cast<double>(a.magnetic.y) / 10;
	double newData_z = static_cast<double>(a.magnetic.z) / 10;

	optime = millis();
	if(optime - last_optime >= 10){	//set frequency of operation
    //shift and insert
		shiftAndInsert(x, signalSize, newData_x);
		shiftAndInsert(y, signalSize, newData_y);
		shiftAndInsert(z, signalSize, newData_z);

    //derivative
		derivative(x, dx, signalSize, timeInterval);
		derivative(y, dy, signalSize, timeInterval);
		derivative(z, dz, signalSize, timeInterval);

		if(optime - last_optime2 >= 1000){  //set frequency of calculations => 0.5 second
      // for(int i = 0 ; i < 300; i++){
      //   Serial.print(x[i]);
      //   Serial.println(" ");
      // }

			//mergeFeatures
			mergeFeatures(features_x, min_ts(x, signalSize), max_ts(x, signalSize), mean_ts(x, signalSize), std_ts(x, signalSize), skew_ts(x, signalSize), kurtosis_ts(x, signalSize), iqr_ts(x, signalSize), area_ts(x, signalSize, timeInterval));
			mergeFeatures(features_y, min_ts(y, signalSize), max_ts(y, signalSize), mean_ts(y, signalSize), std_ts(y, signalSize), skew_ts(y, signalSize), kurtosis_ts(y, signalSize), iqr_ts(y, signalSize), area_ts(y, signalSize, timeInterval));
			mergeFeatures(features_z, min_ts(z, signalSize), max_ts(z, signalSize), mean_ts(z, signalSize), std_ts(z, signalSize), skew_ts(z, signalSize), kurtosis_ts(z, signalSize), iqr_ts(z, signalSize), area_ts(z, signalSize, timeInterval));
			mergeFeatures(features_dx, min_ts(dx, signalSize), max_ts(dx, signalSize), mean_ts(dx, signalSize), std_ts(dx, signalSize), skew_ts(dx, signalSize), kurtosis_ts(dx, signalSize), iqr_ts(dx, signalSize), area_ts(dx, signalSize, timeInterval));
			mergeFeatures(features_dy, min_ts(dy, signalSize), max_ts(dy, signalSize), mean_ts(dy, signalSize), std_ts(dy, signalSize), skew_ts(dy, signalSize), kurtosis_ts(dy, signalSize), iqr_ts(dy, signalSize), area_ts(dy, signalSize, timeInterval));
			mergeFeatures(features_dz, min_ts(dz, signalSize), max_ts(dz, signalSize), mean_ts(dz, signalSize), std_ts(dz, signalSize), skew_ts(dz, signalSize), kurtosis_ts(dz, signalSize), iqr_ts(dz, signalSize), area_ts(dz, signalSize, timeInterval));
			
			//merge_xyz
			merge_xyz(input, features_x, features_y, features_z, features_dx, features_dy, features_dz);
			
			//norms(distances)
      // calculateDistances(distances, input, support_vectors);
      // calculateNorms(distances, norms);

      // //rbf_values
      // calculateLin(norms, lin_values);
      
      // //prediction
      transposeMatrix(support_vectors, transposed_sv);
      dotProduct(input, transposed_sv, K);
      double predict_val = calculatePrediction(w, K);
      Serial.println(predict_val);
      // delay(2000);

			core0_flag = true;
			// Serial.println("core0 flag trueeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
			last_optime2 = optime;
		}

		last_optime = optime;
	}

    //*****************************
    // if(prediction >= 0){
    //     send_SMS();
    //     Serial.println("SMS sent !");
    // }
    //*****************************

    updateSerial();
    // Serial.println("");
    delay(2);
}
