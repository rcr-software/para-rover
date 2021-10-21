#include <Adafruit_GPS.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <stdlib.h>

#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

uint32_t timer = millis();


//RFM 95 pins
#define RFM95_RST 11
#define RFM95_CS 10
#define RFM95_INT 6

//freq being sent 
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT); 

int packetnum = 0;



void setup() {

  GPS_setup();
/*  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); */

  RADIO_initialize();

  }

int send_delay = 2000;

void loop() {
  

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
    ++packetnum;
    float x = (GPS.latitude) * 1000; //so we dont lose any data when float -> int, for accurate values divide lat, long by 1000, alt by 100
    float y = (GPS.longitude) * 1000;
    float z = (GPS.altitude) * 100;

    packet_send(x,y,z); //packs packet #, local RSSI, lat, long, alt into packet and sends over RFM9X 
  
  }
}
