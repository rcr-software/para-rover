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
	motorRL = afms.getMotor(4);
	motorRR = afms.getMotor(3);

	motors[MOTOR_FL] = motorFL;
	motors[MOTOR_FR] = motorFR;
	motors[MOTOR_RL] = motorRL;
	motors[MOTOR_RR] = motorRR;

	setDriveSpeed(0);

	Serial.println(F("DONE"));
}

void DriveNode::setMotorSpeed(int speed, char motorNo)
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

        if (speed < 0)
            speed = -speed;
        if (speed > 255)
            speed = 255;
        if (speed < -255)
            speed = -255;

		
		motors[motorNo]->run(direction);
		motors[motorNo]->setSpeed((uint8_t) speed);
				
	}
}

void DriveNode::setDriveSpeed(int speed)
{
	setDriveSpeed(speed, speed);
}

void DriveNode::setDriveSpeed(int lSpeed, int rSpeed)
{
	setMotorSpeed(lSpeed, MOTOR_FL);
	setMotorSpeed(rSpeed, MOTOR_FR);
	setMotorSpeed(lSpeed, MOTOR_RL);
	setMotorSpeed(rSpeed, MOTOR_RR);
}

void DriveNode::enable(bool enable)
{
	if (!enable)
	{
		setDriveSpeed(0);
	}

	enabled = enable;
}

DriveNode dn;
