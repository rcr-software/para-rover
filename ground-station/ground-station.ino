#include <RH_RF95.h>

#define RFM95_RST 11 // "A"
#define RFM95_CS 10 // "B"
#define RFM95_INT 6 // "D"
#define RF95_FREQ 915.0
RH_RF95 rf95(RFM95_CS, RFM95_INT);

#define BUZZER_PIN 5

// call morse with strings like "..---." to flash that code.
// Yeah, it's not a joke. The serial out is taken up by writing
// data out, so without software serial and an ftdi friend,
// it's this or status LEDs.
// TODO put status LEDs on the board so I can delete this sh*t
const int dot_unit = 50;
void warning(char* str) {
    for (; *str; str++) {
        digitalWrite(LED_BUILTIN, HIGH);
        tone(BUZZER_PIN, 440);
        if (*str == '.') {
            delay(1*dot_unit);
        } else {
            delay(3*dot_unit);
        }
        digitalWrite(LED_BUILTIN, LOW);
        noTone(BUZZER_PIN);
        delay(1*dot_unit);
    }
    delay(7*dot_unit);
}
void error(char* str) {
    while (1) {
        warning(str);
    }
}

void setup()  {
    // Serial is used for 2 way communication with the PC so you cannot debug print.
    // However, there will be support for string packets at some point so those can
    // do the same job. For now, use `warning` instead.
    Serial.begin(115200);

    // initialize the radio
    if (!rf95.init()) {
        error("..");
    }
    if (!rf95.setFrequency(RF95_FREQ)) {
        error("...");
    }

    // optional: high power mode
    //rf95.setTxPower(23, false);
}


// This code is really bulky because I had to write it in non-blocking style.
// TODO Perhaps it would be more elegant to perform the manipulations seperately and
//      handle the non-blocking IO in a different way. Oh well, if this works
//      I'm keeping it, ugly or not.
// TODO The utp/ptu prefix convention is terrible, surely I can stuff those into a struct
//      or something. Really, what was I thinking? I think putting both in their own file
//      with their own private namespace would be a massive improvement.

/*  
UART -> packet protocol: (abbreviated: utp)
Receive 1 unsigned byte which is the length of 
payload (n)
    Receive n bytes that make up the payload
receive 1 unsigned byte as checksum (sum(payload) % 256) 
Transmit payload as packet and restart
*/

/*  
Packet -> UART protocol: (abbreviated: ptu)
Receive packet and length, n
transmit n as unsigned by over serial
for each n bytes in payload:
    transmit over serial
transmit checksum over serial
*/

int transmit_is_complete() {
    return rf95.mode() != rf95.RHModeTx;
}

// packet to uart (ptu) variables
uint8_t ptu_buffer[RH_RF95_MAX_MESSAGE_LEN];
uint8_t ptu_length = 0;
enum {EMPTY, SEND_LEN, IN_PAYLOAD2, SEND_SUM} ptu_state = EMPTY;
int ptu_i = 0;
uint8_t ptu_sum = 0;

// uart to packet (utp) variables
enum {WAIT_LEN, IN_PAYLOAD, WAIT_SUM, TRANSMIT_PACKET} utp_state = WAIT_LEN;
uint8_t utp_length = 0;
int utp_i = 0;
uint8_t utp_sum = 0;
uint8_t utp_buffer[RH_RF95_MAX_MESSAGE_LEN];

void loop() {
    // packet to uart code
    if (ptu_state == EMPTY && rf95.available()) {
        ptu_length = RH_RF95_MAX_MESSAGE_LEN;
        if (rf95.recv(ptu_buffer, &ptu_length)) {
            // if we are empty and rf is available and rf received, then get ready
            // to send over uart
            ptu_state = SEND_LEN;
            ptu_i = 0;
            ptu_sum = 0;
        }
    }
    if (ptu_state == SEND_LEN && Serial.availableForWrite() >= 1) {
        // send the length of the payload to be send over uart
        Serial.write(ptu_length);
        ptu_state = IN_PAYLOAD2;
    }
    if (ptu_state == IN_PAYLOAD2) {
        int max_to_send = Serial.availableForWrite();
        while (max_to_send > 0 && ptu_i < ptu_length) {
            uint8_t c = ptu_buffer[ptu_i];
            Serial.write(c);
            ptu_sum += c;
            max_to_send--;
            ptu_i++;
        }
        if (ptu_i == ptu_length) {
            ptu_state = SEND_SUM;
        }
    }
    if (ptu_state == SEND_SUM && Serial.availableForWrite() >= 1) {
        Serial.write(ptu_sum);
        ptu_state = EMPTY;
    }

    // serial to packet code
    if (utp_state == WAIT_LEN && Serial.available() >= 1 && transmit_is_complete()) {
        utp_length = Serial.read();
        if (utp_length > RH_RF95_MAX_MESSAGE_LEN) {
            error("....");
        }
        utp_i = 0;
        utp_sum = 0;
        utp_state = IN_PAYLOAD;
    }
    if (utp_state == IN_PAYLOAD) {
        int max_to_receive = Serial.available();
        while (max_to_receive > 0 && utp_i < utp_length) {
            uint8_t c = Serial.read();
            utp_buffer[utp_i] = c;
            utp_sum += c;
            max_to_receive--;
            utp_i++;
        }
        if (utp_i >= utp_length) {
            utp_state = WAIT_SUM;
        }
    }
    if (utp_state == WAIT_SUM && Serial.available() >= 1) {
        uint8_t c = Serial.read();
        if (c != utp_sum) {
            warning(".....");
            utp_state = WAIT_LEN;
        } else {
            utp_state = TRANSMIT_PACKET;
        }
    }
    if (utp_state == TRANSMIT_PACKET) {
        rf95.send(utp_buffer, utp_length);
        // note that WAIT_LEN makes sure this packet is done before reading any new data
        utp_state = WAIT_LEN;
    }
}
