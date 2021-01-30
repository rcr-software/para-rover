#include "Arduino.h"
#include "Buzzer.h"
#define DOT_UNIT 50

// Class stucture based on the cannonical tutorial:
// https://www.arduino.cc/en/Hacking/libraryTutorial

// Why? well... It's really nice for audible warnings when somethings gone wrong and debugging time
// sequences (like radio back and forth). Maybe it's foolish but I like beepy tech.

// init function
Buzzer::Buzzer(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
}

// Beeps string (such as ".-.--") once.
void Buzzer::warning(char* str) {
    for (; *str; str++) {
        digitalWrite(LED_BUILTIN, HIGH);
        tone(BUZZER_PIN, 440);
        if (*str == '.') {
            delay(1*DOT_UNIT);
        } else {
            delay(3*DOT_UNIT);
        }
        digitalWrite(LED_BUILTIN, LOW);
        noTone(BUZZER_PIN);
        delay(1*DOT_UNIT);
    }
    // Not 8 because of the 1 above
    delay(7*DOT_UNIT);
}

// same as warning but loops forever.
void Buzzer::error(char* str) {
    while (1) {
        warning(str);
    }
}

