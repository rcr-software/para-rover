#include "imu_mag.h"

void IMU::init() {
  lsm6ds_success = lsm6ds.begin_I2C();
  lis3mdl_success = lis3mdl.begin_I2C();
  
  
  if (!lsm6ds_success){
    Serial.println("Failed to find LSM6DS chip");
  }
  if (!lis3mdl_success){
    Serial.println("Failed to find LIS3MDL chip");
  }
  if (!(lsm6ds_success && lis3mdl_success)) {
    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DS and LIS3MDL Found!");

  /*initializing accelerometer settings*/
  lsm6ds.setAccelRange(LSM6DS_ACCEL_RANGE_8_G);               //2,4,8,16 [G]
  lsm6ds.setAccelDataRate(LSM6DS_RATE_6_66K_HZ);              //0, 12.5, 26, 104, 208, 416, 833, 1.66K, 3.33K, 6.66K_HZ [HZ]

  /*initializing gyroscope settings*/
  lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );            //125, 250, 500, 1000, 2000, 4000 [degrees/s]
  lsm6ds.setGyroDataRate(LSM6DS_RATE_6_66K_HZ);               //0, 12.5, 26, 104, 208, 416, 833, 1.66K, 3.33K, 6.66K_HZ [HZ] 
}

void IMU::initGyroAngle() {
  for (int i = 0; i < 99; i++) {
    lsm6ds.getEvent(&accel, &gyro, &temp);
    gyroAngleX+=getAccelX()/100;
    gyroAngleY+=getAccelY()/100;

    delay(1);
  }
}

double IMU::getAccelX() {
  lsm6ds.getEvent(&accel, &gyro, &temp);
  double AccelX = (atan2(accel.acceleration.z,accel.acceleration.y)*(-1)*RAD_TO_DEG)+180;
  return AccelX;
}

double IMU::getAccelY() {
  lsm6ds.getEvent(&accel, &gyro, &temp);
  double AccelY = (atan2(accel.acceleration.z,accel.acceleration.x)*RAD_TO_DEG);
  return AccelY;
}

void IMU::getGyroX() {
  lsm6ds.getEvent(&accel, &gyro, &temp);

  elapsedTime = timer();
  
  gyroAngleX += (gyro.gyro.x*RAD_TO_DEG)*elapsedTime - initGyroBias;
  gyroAngleY += (gyro.gyro.y*RAD_TO_DEG)*elapsedTime - initGyroBias; //initGyroBias NOT TESTED
}
