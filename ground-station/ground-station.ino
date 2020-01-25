#include <RH_RF95.h>

#include <rover_packet_types.h>

// radio setup section
#define ROVER 0

#if ROVER
    // for embedded radio boi
    #define RFM95_RST     4   // "A"
    #define RFM95_CS      8   // "B"
    #define RFM95_INT     3    // "D"
#else
    // for seperate radio boi
    #define RFM95_RST     11   // "A"
    #define RFM95_CS      10   // "B"
    #define RFM95_INT     6    // "D"
#endif

#define RF95_FREQ 915.0
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// late include :(
#include <struct_send_receive.h>

// setup/loop entry section
void setup()  {
    #if ROVER
    Serial.println("GOURD STATION FOR ROVER");
    #else
    Serial.println("GOURD STATION FOR PARACHUTE CONTROLLER");
    #endif

    //setup pins
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    Serial.begin(115200);

    // manual reset the radio
    digitalWrite(RFM95_RST, LOW); delay(100);
    digitalWrite(RFM95_RST, HIGH); delay(100);

    // initialize the radio
    if (!rf95.init()) {
        Serial.println("Init failed");
        while(1);
    }
    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("Freq setting failed");
        while(1);
    }

    // optional: high power mode
    //rf95.setTxPower(23, false);

    Serial.println("Setup complete.");
    delay(100);
}

void loop()  {
    //Serial.println(millis());

    //digitalWrite(LED_BUILTIN, LOW);
    if (Serial.available() > 0) {
        char in_char = Serial.read();
        switch (in_char) {
            case 'w':
                Serial.println("w going forward");
                send_joystick(1.0, 0.0, 0.0, 0.0);
                break;
            case 'a':
                Serial.println("a going left");
                send_joystick(0.0, -1.0, 0.0, 0.0);
                break;
            case 's':
                Serial.println("s going backward");
                send_joystick(-1.0, 0.0, 0.0, 0.0);
                break;
            case 'd':
                Serial.println("d going rigth");
                send_joystick(0.0, 1.0, 0.0, 0.0);
                break;
            default:
                Serial.write(in_char);
                Serial.println(" not recognized");
                break;
        }
    }
    if (rf95.available()) {
        try_receive_message();
    }
}
