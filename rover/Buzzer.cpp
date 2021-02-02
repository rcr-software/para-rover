#include "Arduino.h"
#include "Buzzer.h"


/* Example Usage:
#include "Buzzer.h"
Buzzer buzzer(5);
void init() {}
void loop() {
    if (notable_thing)
        buzzer.warning(".");
    if (mildly_bad_thing)
        buzzer.warning("--");
    if (unfixable_bad_thing)
        buzzer.warning("---...");
}
*/

// this is how many milliseconds a dot takes.
#define DOT_UNIT 50

// Class stucture based on the cannonical tutorial:
// https://www.arduino.cc/en/Hacking/libraryTutorial

// Why do we buzz? well... It's really nice for audible warnings when somethings gone wrong and
// debugging time sequences (like radio back and forth). Maybe it's foolish but I like beepy tech.
// Helpful so you don't have to always have serial moniter connected. Problematic because it spends
// a LONG time busy when you warn, so motors stall and incoming radio packets get missed.

// init function
Buzzer::Buzzer(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
}

// Beeps string (such as ".-.--") once.
void Buzzer::warning(char* str) {
    for (; *str; str++) {
        //digitalWrite(LED_BUILTIN, HIGH);
        tone(_pin, 440);
        if (*str == '.') {
            delay(1*DOT_UNIT);
        } else {
            delay(3*DOT_UNIT);
        }
        //digitalWrite(LED_BUILTIN, LOW);
        noTone(_pin);
        delay(1*DOT_UNIT);
    }
    // Not 8 because of the 1 above
    delay(7*DOT_UNIT);
}

// same as warning but loops forever.
void Buzzer::error(char* str) {
    while (1) {
        this->warning(str);
    }
}

