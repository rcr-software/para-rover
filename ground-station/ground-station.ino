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

void loop()  {
    if (Serial.available() > 0) {
        unsigned char len = Serial.read();
        Serial.print("taking in serial of len");
        Serial.println(len);
        for (int i = 0; i < len; i++) {
            while (Serial.available() <= 0); // block until serial available
            radio_transmit_buffer[i] = Serial.read();
            Serial.print(radio_transmit_buffer[i], HEX);
            Serial.print(" ");
        }
        Serial.println("\nFEEEE");
        print_struct(radio_transmit_buffer);
        Serial.println("EEEEF");
        Serial.println("\nDone, radio transmitting");
        rf95.send(radio_transmit_buffer, len);
        rf95.waitPacketSent();
        Serial.println("Daone transmitting");
    }
    if (rf95.available()) {
        try_receive_message();
    }
}
