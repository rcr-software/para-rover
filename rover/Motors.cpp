#include "Arduino.h"
#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include "Motors.h"

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

// These are intentionally left out of the class... hmm wait... TODO uh
void left_one_step_forward() { left_stepper->onestep(FORWARD, STEP_TYPE); }
void left_one_step_backward() { left_stepper->onestep(BACKWARD, STEP_TYPE); }
void right_one_step_forward() { right_stepper->onestep(FORWARD, STEP_TYPE); }
void right_one_step_backward() { right_stepper->onestep(BACKWARD, STEP_TYPE); }

// TODO figure out the type of this so I can take it as an argument
#define STEP_TYPE INTERLEAVE

// Notice the object composition syntax
Motors::Motors(int steps_per_revolution, int max_speed) :
    Adafruit_MotorShield motor_shield(),
    AccelStepper left_motor (left_one_step_forward, left_one_step_backward),
    AccelStepper right_motor (right_one_step_forward, right_one_step_backward)
{
        
    Adafruit_StepperMotor *right_stepper = motor_sheild.getStepper(200, 2);
    Adafruit_StepperMotor *left_stepper = motor_sheild.getStepper(200, 1);
    AccelStepper left_motor = AccelStepper::AccelStepper(left_one_step_forward, left_one_step_backward);
    AccelStepper right_motor = AccelStepper::AccelStepper(right_one_step_forward, right_one_step_backward);

}

void Motors::drive(int left_speed, int right_speed) {
    // TODO checking for mode==TELEOP isn't great, there should be
    // some sort of disable mechanism that's controlled by the modes.
    //if (mode == TELEOP) {
        left_motor.setSpeed(left_speed);
        right_motor.setSpeed(right_speed);
    //}
}

