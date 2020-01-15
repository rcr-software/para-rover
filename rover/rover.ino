/*---------------------------------------------------------------
*	rover.ino
*	Author: Ben Ferguson
*	Date created: 1-14-20
*	Date modified: 1-14-20
*	Description: main logic for the rover micro
---------------------------------------------------------------*/

#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

long lidarGetRangemm()
{
	VL53L0X_RangingMeasurementData_t measure;

	lox.rangingTest(&measure, false);

	if (measure.RangeStatus != 4) {
		return measure.RangeMilliMeter;
	} 

	else {
		return -1;
	}
}

void setup() {
  Serial.begin(9600);

  while (!Serial){
  	delay(1);
  }

  Serial.println(F("Initializing..."));

  Serial.print(F("Initialzing VL53L0X..."));

  if (!lox.begin()) {
    Serial.println(F("FAIL"));
    while(1);
  }

  Serial.println(F("DONE"));

  Serial.println(F("Initialization complete!"));
}

void loop() {
  
}
