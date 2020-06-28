#include <Adafruit_MotorShield.h>

class Drive
{
	public:
        Drive();
		void drive(int left_speed, int right_speed);
	private:
		Adafruit_MotorShield motor_shield;
		Adafruit_DCMotor *motor_left; 
		Adafruit_DCMotor *motor_right;

        int abs_clamp(int x, int upper_limit);
        uint8_t speed_to_direction(int x);
};
