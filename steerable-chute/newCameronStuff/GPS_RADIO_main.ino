//up to date 2-19-22
//included functionality for bno and bmp sensors
//log all data to sd card
#include <Adafruit_GPS.h>           
#include <SPI.h>
#include <RH_RF95.h>
#include <stdlib.h>
#include <SD.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
//look at main in git for kalman filter

#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

uint32_t timer = millis();

const int chipSelect = 4;

uint8_t even = 0;


//RFM 95 pins
#define RFM95_RST 11
#define RFM95_CS 10
#define RFM95_INT 6

//freq being sent 
#define RF95_FREQ 915.0

//object initializations
RH_RF95 rf95(RFM95_CS, RFM95_INT); 
Adafruit_BMP280 bmp;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

int packetnum = 0;
float sea_level_pressure = 0;



void setup() {
  
  Serial.begin(11250);
  
  GPS_setup();

  //sd_init();

  bmp_init_check();
  bmp_set_presets();
  getPressure();

  bno_init_check();
  

  RADIO_initialize();

  }

int send_delay = 500;

void loop() {
  //bmp and bno block
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

  //printEvent(&orientationData);
  Serial.println("--");
  //Serial.print("\nBMP280's altitude = ");
  float bmp_alt = bmp.readAltitude(sea_level_pressure);
  //Serial.print(bmp_alt);
  //Serial.print("\n\n\n");

  //get yaw/pitch/roll for sd logging
  //implement with mr kalman
  float YAW = returnYAW(&orientationData);
  float PITCH = returnPITCH(&orientationData);
  float ROLL = returnROLL(&orientationData);



  

  
  

  char c = GPS.read();

  if (GPS.newNMEAreceived()) {

    Serial.print(GPS.lastNMEA());
    if(!GPS.parse(GPS.lastNMEA()))
    return;
  }

  if (millis() - timer > send_delay) {

    timer = millis(); 

    GPS_print(); //prints GPS data to serial monitor
    
    RADIO_waitresponse(); //radio listens for ping from ground station to get local RSSI value
    
    float x = (GPS.latitude) * 1000; //so we dont lose any data when float -> int, for accurate values divide lat, long by 1000, alt by 100
    float y = (GPS.longitude) * 1000;
    float z = (GPS.altitude) * 1000;


//    float x = 100;
//    float y = 100;
//    float z = 100;
 

    packet_send(x,y,z); //packs packet #, local RSSI, lat, long, alt into packet and sends over RFM9X 
    sd_logdata(x,y,z,bmp_alt,YAW,PITCH,ROLL);
  }

}
