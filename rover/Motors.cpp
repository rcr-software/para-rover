#include <Arduino.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

/* Example Usage:
#include "Motors.h"
void init() {
    Motors::init();
}

void loop() {
    if (joystick_input) {
        Motors::drive(x, y);
    }

    if (driving_mode) {
        // Move at a set speed
        Motors::runSpeed();
    } else if (dancing_mode) {
        // Move to random positions
        Motors::dance();
        Motors::run();
    } else {
        // Stop supplying power to the motors to conserve energy or allow coasting
        Motors::release();
    }
}

=== NOTE: =====
Notice that the run functions have to be called repeatedly - if your code doesn't run them
constantly, then the motors won't step until they're called again.
Also be careful not to call both runSpeed and run in the same loop. They'll fight and create erratic
behavior. I had this bug until I added drive disabling and the current mode switching structure.
===============
*/

//step options: SINGLE, DOUBLE (more torque), INTERLEAVE (half speed, smoother), MICROSTEP
#define STEP_TYPE INTERLEAVE

namespace Motors {
    Adafruit_MotorShield motor_shield = Adafruit_MotorShield();
    // 200 steps per revolution  = 1.8 degree steppers
    Adafruit_StepperMotor *right_stepper = motor_shield.getStepper(200, 2);
    Adafruit_StepperMotor *left_stepper = motor_shield.getStepper(200, 1);

    // Create 4 functions to be called by AccelStepper. AccelStepper has a functional and a
    // pin-based intialization, and only the functional one can be used with an
    // Adafruit_StepperMotor since it's i2c and not GPIO pins.
    void left_one_step_forward() { left_stepper->onestep(FORWARD, STEP_TYPE); }
    void left_one_step_backward() { left_stepper->onestep(BACKWARD, STEP_TYPE); }
    void right_one_step_forward() { right_stepper->onestep(FORWARD, STEP_TYPE); }
    void right_one_step_backward() { right_stepper->onestep(BACKWARD, STEP_TYPE); }
    AccelStepper left_motor(left_one_step_forward, left_one_step_backward);
    AccelStepper right_motor(right_one_step_forward, right_one_step_backward);

    // non-blocking update function for using moveTo calls (like in dance)
    void run() {
        left_motor.run();
        right_motor.run();
    }

    // non-blocking update function for using setSpeed calls (like in drive)
    void runSpeed() {
        left_motor.runSpeed();
        right_motor.runSpeed();
    }

    // motors draw no power
    void release() {
        left_stepper->release();
        right_stepper->release();
    }

    // motors are set to random positions, call both dance and runSpeed in a loop to use.
    void dance() {
        if (left_motor.distanceToGo() == 0)
            left_motor.moveTo(random(-200, 200));
        if (right_motor.distanceToGo() == 0)
            right_motor.moveTo(random(-200, 200));
    }

    // set motor drive speed. Call this once then call runSpeed in a loop.
    void drive(int left_speed, int right_speed) {
        left_motor.setSpeed(left_speed);
        right_motor.setSpeed(right_speed);
    }

    void init() {
        motor_shield.begin();
        // This max speed is about as fast as it seems to want to run, but I think it could be run
        // much fast if stepped manually. I haven't fiddled with acceleration.
        left_motor.setMaxSpeed(9000.0);
        left_motor.setAcceleration(100.0);
        right_motor.setMaxSpeed(9000.0);
        right_motor.setAcceleration(100.0);
        drive(0, 0);
    }
}
