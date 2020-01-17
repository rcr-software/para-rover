/*---------------------------------------------------------------
*	rover.ino
*	Author: Ben Ferguson
*	Description: main logic for the rover micro
---------------------------------------------------------------*/

#include "src/main.h"

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  while (!Serial){
  	delay(1);
  }

  Serial.println(F("Initializing..."));

  sensing::init();

  Serial.println(F("Initialization complete!"));
}

void loop() {

}
