/*--------------------------------------------------------------
*	DriveNode.cpp
*	Author: Ben Ferguson
*	Description: Class implementation for the drive-train node
--------------------------------------------------------------*/

#include "DriveNode.h"

void DriveNode::init()
{
	enabled = true;

	Serial.print(F("Initializing drive-train... "));

	afms.begin();

	motorFL = afms.getMotor(1);
	motorFR = afms.getMotor(2);
	motorRL = afms.getMotor(3);
	motorRR = afms.getMotor(4);

	motors[MOTOR_FL] = motorFL;
	motors[MOTOR_FR] = motorFR;
	motors[MOTOR_RL] = motorRL;
	motors[MOTOR_RR] = motorRR;

	setDriveSpeed(0);

	Serial.println(F("DONE"));
}

void DriveNode::setMotorSpeed(uint8_t speed, char motorNo)
{
	if (enabled)
	{
		uint8_t direction;

		if (speed > 0)
		{
			direction = FORWARD;
		}

		else if (speed < 0)
		{
			direction = BACKWARD;
		}

		else
		{
			direction = RELEASE;
		}
		
		motors[motorNo]->run(direction);
		motors[motorNo]->setSpeed(speed);
				
	}
}

void DriveNode::setDriveSpeed(uint8_t speed)
{
	setDriveSpeed(speed, speed);
}

void DriveNode::setDriveSpeed(uint8_t lSpeed, uint8_t rSpeed)
{
	setMotorSpeed(lSpeed, MOTOR_FL);
	setMotorSpeed(rSpeed, MOTOR_FR);
	setMotorSpeed(lSpeed, MOTOR_RL);
	setMotorSpeed(rSpeed, MOTOR_RL);
}

void DriveNode::enable(bool enable)
{
	if (!enable)
	{
		setDriveSpeed(0);
	}

	enabled = enable;
}