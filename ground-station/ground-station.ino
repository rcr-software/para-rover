#include <RH_RF95.h>

#include "rover_packet_types.h"

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
#include "struct_send_receive.h"


uint8_t radio_transmit_buffer[RH_RF95_MAX_MESSAGE_LEN];

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

// 'b' is terminator, 'a' is escape char.
// Thus, ab->b, aa->a, and unescaped 'b' means finish and send
int i = 0;
bool escaped = 0;
void loop() {
    if (rf95.available()) {
        try_receive_message();
    }

    if (Serial.available() > 0) {
        char inchar = Serial.read();
        // if not escaped and 'a', then escape
        if (!escaped && inchar == 'a') {
            escaped = 1;
            // if not escaped and 'b', then end and send and reset
        } else if (!escaped && inchar == 'b') {
            int length = i; // of by one?
            Serial.println("\nSending this struct with len=");
            Serial.println(length);
            // TODO ideally ground station is agnostic to the packet structs,
            // or print_struct is auto-built from them somehow?
            print_struct(radio_transmit_buffer);
            Serial.println("EEEEF");
            Serial.println("\nDone, radio transmitting");
            rf95.send(radio_transmit_buffer, length);
            rf95.waitPacketSent();
            i = 0;
        } else {
            radio_transmit_buffer[i] = inchar;
            i++;
            escaped = 0;
        }
    }
}
