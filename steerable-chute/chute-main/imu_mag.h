#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_LIS3MDL.h>

#ifndef _IMUMAG_H
#define _IMU_H

//lsm6ds.getEvent(&accel, &gyro, &temp);

extern double timer();
extern double elapsedTime, prevTime, currTime, startup;

/* ENABLES IMU MODULE */
class IMU {
//private:
  bool lsm6ds_success, lis3mdl_success;
  float accelAngleX;
  float accelAngleY;
  float gyroAngleX;
  float gyroAngleY;
  float initGyroBias;
  float elapsedTime;

public:
/*FUNCTION PROTOTYPES*/
  void init(void);                                        //initializes accelerometer & gyroscope module
  void initGyroAngle(void);                               //sets initial Gyro Angles from 100x sample of accelerometer angles
  void gyroBias(void);                                    //finds gyro bias to subtract from angle
  
  double getAccelX(void);                                 //sets accelerometer X angle  (degrees)
  double getAccelY(void);                                 //sets accelerometer Y angle  (degrees)
  void getGyroX(void);                                    //sets gyro X angle           (degrees)
  void getGyroY(void);                                    //sets gyro Y angle           (degrees)

};

class Mag {
//private:

public:
  void init(void);
  
};

extern IMU imu;
extern Mag magno;
extern Adafruit_LSM6DSOX lsm6ds;
extern Adafruit_LIS3MDL lis3mdl;
extern sensors_event_t accel, gyro, temp;

#endif //_IMUMAG_H
