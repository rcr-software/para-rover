/*-----------------------------------------------------------
*	Sensing.h
*	Author: Ben Ferguson
*	Description: Declarations for the sensing library
-----------------------------------------------------------*/

#ifndef SENSING_H_
#define SENSING_H_

#include <Adafruit_VL53L0X.h>
#include <Adafruit_BMP085.h>

extern Adafruit_VL53L0X lox;
extern Adafruit_BMP085 bmp;

namespace sensing
{
	void init();

	long lidarGetRangemm();
}

#endif