/* ADDITIONAL STEPS
 *  1) In preferences, add the following link to "Additional Boards Managers URL": https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
 *  2) Libraries: Adafruit LIS3MDL, LSM6DS, BusIO, Adafruit Unified Sensor
 *  3) For the RFM9x, download: https://cdn-learn.adafruit.com/assets/assets/000/035/106/original/RadioHead-1.62.zip?1472068723 (RadioHead)
 */

#include <Adafruit_LSM6DSOX.h>
Adafruit_LSM6DSOX lsm6ds;
 
#include <Adafruit_LIS3MDL.h>
Adafruit_LIS3MDL lis3mdl;

#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

#define GPSSerial Serial1

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

 
void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  //Initializing RFM9x
  Initialize_RFM();
  
  //Initializing IMU
  Initialize_IMU();
  Initialize_Accel();
  Initialize_Gyro();
  Initialize_Mag();

  //Initializing GPS
  GPSSerial.begin(9600);
}
 
int16_t packetnum = 0;  // packet counter, we increment per xmission
 

void loop() {

//  printIMU();

//  helloWorld(); //Tests if RFM9x is working

//  printGPS();  //Tests if GPS is working
  delay(100);
 
}

void printIMU() {
  sensors_event_t accel, gyro, mag, temp;
  
  //  /* Get new normalized sensor events */
  lsm6ds.getEvent(&accel, &gyro, &temp);
  lis3mdl.getEvent(&mag);
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tAccel X: ");
  Serial.print(accel.acceleration.x, 4);
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y, 4);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z, 4);
  Serial.println(" \tm/s^2 ");
 
  /* Display the results (rotation is measured in rad/s) */
  Serial.print("\t\tGyro  X: ");
  Serial.print(gyro.gyro.x, 4);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y, 4);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z, 4);
  Serial.println(" \tradians/s ");
 
  /* Display the results (magnetic field is measured in uTesla) */
  Serial.print(" \t\tMag   X: ");
  Serial.print(mag.magnetic.x, 4);
  Serial.print(" \tY: ");
  Serial.print(mag.magnetic.y, 4);
  Serial.print(" \tZ: ");
  Serial.print(mag.magnetic.z, 4);
  Serial.println(" \tuTesla ");
 
  Serial.print("\t\tTemp   :\t\t\t\t\t");
  Serial.print(temp.temperature);
  Serial.println(" \tdeg C");
  Serial.println();
}
