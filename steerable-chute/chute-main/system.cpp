#include "system.h"
sensors_event_t accel, gyro, temp;          //IMU Sensor Data Objects
IMU imu;                                    //IMU Big Boy Object

void SYSTEM_Initialize() {
  imu.init();
  imu.initGyroAngle();
  //imu.gyroBias();

  startup = millis();
}
