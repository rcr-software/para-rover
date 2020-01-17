/*-----------------------------------------------------------------
*	Sensing.cpp
*	Author: Ben Ferguson
*	Description: Implementation for sensing library
-----------------------------------------------------------------*/

#include "Sensing.h"

Adafruit_VL53L0X lox;
Adafruit_BMP085 bmp;

namespace sensing
{
	void init()
	{
		Serial.print(F("Initialzing VL53L0X..."));

		if (!lox.begin()) {
		Serial.println(F("FAIL"));
		while(true);
		}

		Serial.println(F("DONE"));

		Serial.print(F("Initializing BMP180... "));

		if (!bmp.begin()) {
		Serial.println("FAIL");
		while (true);
		}

		Serial.println(F("DONE"));
	}

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
}