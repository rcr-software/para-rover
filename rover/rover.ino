#include "packets.h"
#include <Wire.h>
#include <RH_RF95.h>
#include "Motors.h"
#include "Buzzer.h"

// Radio wiring and setup
#define RFM95_RST 11 // "A"
#define RFM95_CS 10 // "B"
#define RFM95_INT 6 // "D"
#define RF95_FREQ 915.0
RH_RF95 rf95(RFM95_CS, RFM95_INT);
uint8_t receive_buffer[RH_RF95_MAX_MESSAGE_LEN];
uint8_t receive_size;
uint8_t transmit_buffer[RH_RF95_MAX_MESSAGE_LEN];
uint8_t transmit_size;

// Buzzer setup
Buzzer buzzer(5);

// Global for general control mode
enum {TELEOP, DANCE, SLEEP} mode = TELEOP;



// if this is too large, then turn off the motors.
int last_joystick_packet = 0;

void dispatch() {
    int packet_number = *((int *)receive_buffer);
    // NOTE this has to be an if/else so that variable can be declared inside
    if (packet_number == PAC_STRING) {
        Serial.println("Got a string, printing");
        Serial.println(((string_t*)receive_buffer)->string);
    } else if (packet_number == PAC_TRIPLET) {
        triplet_t* triplet;
        Serial.println("Got a triplet");
        triplet = (triplet_t *)receive_buffer; 
    } else if (packet_number == PAC_JOYSTICK) {
        last_joystick_packet = millis();
        mode = TELEOP;
        joystick_t* joystick = (joystick_t*) receive_buffer; 
        Serial.println((int) joystick->y1);
        Serial.println((int) joystick->y2);
        Motors::drive(joystick->y1, joystick->y2);
    } else if (packet_number == PAC_SENSOR) {
        Serial.println("Got a sensor? We're supposed to send those, not receive them.");
    } else if (packet_number == PAC_BUTTON) {
        button_t* button = (button_t*) receive_buffer;
        button_code_t button_code = (button_code_t) button->button;
        if (button_code == BUTTON_SELECT) {
            mode = SLEEP;
        } else if (button_code == BUTTON_START) {
            mode = TELEOP;
        } else if (button_code == BUTTON_X) {
            mode = DANCE;
        } else {
            buzzer.warning("--");
        }
    } else {
        Serial.println("TF is this??");
        buzzer.warning(".--");
    }
}

void setup() {
    Serial.println("Initializing...");
    Serial.begin(9600);

    // Radio init
    if (!rf95.init()) {
        buzzer.error("..");
    }
    if (!rf95.setFrequency(RF95_FREQ)) {
        buzzer.error("---");
    }

    Motors::init();

    Serial.println("Initialization complete!");
}

int interval_start = 0;

void loop() {
    // TODO configure watchdog timer to reset in case some code is
    // accidentally blocking or (more likely) crashes the board.

    // If we haven't seen a joystick packet, stop driving. TODO is this smart?
    if (millis() - last_joystick_packet > 500) {
        Motors::drive(0, 0);
        mode = SLEEP;
    }

    // In this mode, the motors run at a set speed, controlled by the joystick
    // TODO bug: under battery power, the left motor periodically stops at equally spaced intervals
    //            about 20 degrees apart
    if (mode == TELEOP) {
        Motors::runSpeed();
    }

    // In sleep mode, the motors are released so they don't draw any current
    if (mode == SLEEP) {
        Motors::release();
    }

    // In this mode, we use run() instead of runSpeed() and seek random positions.
    // TODO bug: often only one motor runs, but sometimes both do.
    if (mode == DANCE) {
        Motors::dance();
        Motors::run();
    }

    // If available, receive a packet and act according to what's inside it
    // TODO add an "echo" protocol where a checksum is sent back after receiving
    //      so the ground station knows when messages aren't being received.
    if (rf95.available()) {
        receive_size = RH_RF95_MAX_MESSAGE_LEN;
        if (rf95.recv(receive_buffer, &receive_size)) {
            dispatch();
        }
    }

    // Every 10 seconds, send a test packet to the groundstation
    // In the future, this will be sensor data instead.
    if (millis() - interval_start > 10000) {
        interval_start = millis();
        // TODO uncomment this, repair it, and test it
        triplet_t foo;
        foo.type_spec = 1;
        foo.a = 1;
        foo.b = 2;
        foo.c = 3;
        // Horray for double unsafe cast!
        memcpy(transmit_buffer, &foo, sizeof(foo));
        transmit_size = sizeof(foo);
        rf95.send(transmit_buffer, sizeof(foo));
    }
}
