#include <Arduino.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

// Motor shield and stepper setup
// 200 steps per revolution  = 1.8 degree steppers

// AccelStepper (using steppers like DC motors) setup
/* 
Usage instructions (using right_motor and right_stepper as an exmaple WLOG):
To drive at a set speed:
    right_motor.setSpeed(200)
    in loop: right_motor.runSpeed()
To rotate to a specific location:
    right_motor.moveTo(1200)
    in loop: right_motor.run()
Notice that the run functions have to be called repeatedly - if your code doesn't
run them constantly, then the motors won't step until they're called again.
Also be careful not to call both of them in the same loop. They'll fight and create erratic
behavior. I had this bug until I added drive disabling and the current mode switching structure.
*/
//step options: SINGLE, DOUBLE (more torque), INTERLEAVE (half speed, smoother), MICROSTEP
#define STEP_TYPE INTERLEAVE

namespace Motors {
    Adafruit_MotorShield motor_shield = Adafruit_MotorShield(); 
    Adafruit_StepperMotor *right_stepper = motor_shield.getStepper(200, 2);
    Adafruit_StepperMotor *left_stepper = motor_shield.getStepper(200, 1);
    void left_one_step_forward() { left_stepper->onestep(FORWARD, STEP_TYPE); }
    void left_one_step_backward() { left_stepper->onestep(BACKWARD, STEP_TYPE); }
    void right_one_step_forward() { right_stepper->onestep(FORWARD, STEP_TYPE); }
    void right_one_step_backward() { right_stepper->onestep(BACKWARD, STEP_TYPE); }
    AccelStepper left_motor(left_one_step_forward, left_one_step_backward);
    AccelStepper right_motor(right_one_step_forward, right_one_step_backward);

    void run() {
        left_motor.run();
        right_motor.run();
    }

    void runSpeed() {
        left_motor.runSpeed();
        right_motor.runSpeed();
    }

    void release() {
        left_stepper->release();
        right_stepper->release();
    }

    void dance() {
        if (left_motor.distanceToGo() == 0)
            left_motor.moveTo(random(-200, 200));
        if (right_motor.distanceToGo() == 0)
            right_motor.moveTo(random(-200, 200));
    }

    void drive(int left_speed, int right_speed) {
        left_motor.setSpeed(left_speed);
        right_motor.setSpeed(right_speed);
    }

    void init() {
        motor_shield.begin();
        left_motor.setMaxSpeed(100.0);
        left_motor.setAcceleration(100.0);
        right_motor.setMaxSpeed(100.0);
        right_motor.setAcceleration(100.0);
        drive(0, 0);
    }


}
