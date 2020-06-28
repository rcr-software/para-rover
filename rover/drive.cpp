#include "Arduino.h"
#include "drive.h"

// initializer
Drive::Drive() {
	motor_shield.begin();
    
    // these might need fiddling if you re-wire the motors
	motor_left = motor_shield.getMotor(3);
	motor_right = motor_shield.getMotor(4);
}

int Drive::abs_clamp(int x, int upper_limit) {
    x = abs(x);
    if (x > upper_limit)
        return upper_limit;
    return x;
}

uint8_t Drive::speed_to_direction(int x) {
    if (x < 0)
        return BACKWARD;
    if (x > 0)
        return FORWARD;
    return RELEASE;
}

void Drive::drive(int left_speed, int right_speed) {
    // these might need fiddling if you rewire motors
    left_speed = -left_speed;
    right_speed = -right_speed;

    motor_left->setSpeed(0);
    motor_left->run(speed_to_direction(left_speed));
    motor_left->setSpeed(abs_clamp(left_speed, 255));

    motor_right->setSpeed(0);
    motor_right->run(speed_to_direction(right_speed));
    motor_right->setSpeed(abs_clamp(right_speed, 255));
}
        

    
