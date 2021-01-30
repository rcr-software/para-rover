#ifndef Buzzer_h
#define Buzzer_h

#include "Arduino.h"

class Morse
{
  public:
    Buzzer(int pin);
    void warning();
    void error();
  private:
    int _pin;
};

#endif
