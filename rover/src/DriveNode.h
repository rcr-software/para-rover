/*-----------------------------------------------------------
*	DriveNode.h
*	Author: Ben Ferguson
*	Description: Class declaration for the drive-train node
-----------------------------------------------------------*/

#ifndef DRIVENODE_H_
#define DRIVENODE_H_

#include "main.h"
#include <Adafruit_MotorShield.h>

enum enumMotors{MOTOR_FL, MOTOR_FR, MOTOR_RL, MOTOR_RR};

class DriveNode
{
	private:
		Adafruit_MotorShield afms;

		Adafruit_DCMotor *motorFL; 

		Adafruit_DCMotor *motorFR;
		
		Adafruit_DCMotor *motorRL;

		Adafruit_DCMotor *motorRR;

		Adafruit_DCMotor *motors[4];

		bool enabled;

	public:
		void init();

		void setMotorSpeed(uint8_t speed, char motorNo);

		void setDriveSpeed(uint8_t speed);

		void setDriveSpeed(uint8_t lSpeed, uint8_t rSpeed);

		void enable(bool enable);

};

#endif