#ifndef Buzzer_h
#define Buzzer_h

#include "Arduino.h"

class Buzzer
{
  public:
    Buzzer(int pin);
    void warning(char*);
    void error(char*);
  private:
    int _pin;
};

#endif
