
#include "Arduino.h"
// TODO get rid of unneeded includes
#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

class Motors
{
  public:
    Motors(int steps_per_revolution, int max_speed);
    void warning();
    void error();
  private:
    int _pin;
};
