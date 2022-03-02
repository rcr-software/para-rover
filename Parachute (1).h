#pragma once

enum Dir
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	UNKOWN
}

class Parachute
{
public:
	Parachute(Adafuit_gps GPS, Adafruit_BMP280 bmp);
	bool calibrate(int spot, bool isPlayer);
};