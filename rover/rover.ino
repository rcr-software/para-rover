/*---------------------------------------------------------------
*	rover.ino
*	Author: Ben Ferguson
*	Description: main logic for the rover micro
---------------------------------------------------------------*/

#include "src/main.h"

DriveNode dn;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  while (!Serial){
  	delay(1);
  }

  Serial.println(F("Initializing..."));

  sensing::init();

  dn.init();

  Serial.println(F("Initialization complete!"));
}

void loop() {

}
