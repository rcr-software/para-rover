#include "system.h"

/**********************DEBUG MODES**********************/
//#define DEBUG_ADALOGGER
//#define DEBUG_GPS
//#define DEBUG_IMU
//#define DEBUG_MAG
//#define DEBUG_PWM
//#define DEBUG_RFM
//#define DEBUG_STEPMOT
/********************END DEBUG MODES********************/

/******************GLOBAL DECLARATIONS******************/
Adafruit_LSM6DSOX lsm6ds;    //accel + gyro
Adafruit_LIS3MDL lis3mdl;    //magnometer
double elapsedTime = 0.1;    //CALCULATE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
double prevTime = 0;
double currTime = 0;
double startup = 0;
/****************END GLOBAL DECLARATIONS****************/

void timer(void);    //TIME PER LOOP from the moment the function was initialized until it runs again

/* _____      _
 / ____|    | |
| (___   ___| |_ _   _ _ __
\___ \ / _ \ __| | | | '_ \
____) |  __/ |_| |_| | |_) |
|_____/ \___|\__|\__,_| .__/
            | |
            |_|*/
void setup() {
  SYSTEM_Initialize();
}

/********************END SETUP FUNCTION********************/


/*__  __       _         _
|  \/  |     (_)       | |
| \  / | __ _ _ _ __   | |     ___   ___  _ __
| |\/| |/ _` | | '_ \  | |    / _ \ / _ \| '_ \
| |  | | (_| | | | | | | |___| (_) | (_) | |_) |
|_|  |_|\__,_|_|_| |_| |______\___/ \___/| .__/
                     | |
                     |_| */
void loop() {
  //main loop
}

double timer(void) {
  prevTime = currTime;
  currTime = millis();
  currTime -= startup;
  elapsedTime = (currTime - prevTime) / 1000;
  return elapsedTime;
}
