#include "Arduino.h"
#include "radio.h"

// Initializer takes three pin numbers and freq
Radio::Radio(uint8_t rst, uint8_t cst, uint8_t intr, double freq)
{
    receive_buffer = (uint8_t*) malloc(RH_RF95_MAX_MESSAGE_LEN * sizeof(uint8_t));

    reset_pin = rst; // save to attribute
    Serial.print("Initializing radio... ");
    rf95 = new RH_RF95(cst, intr);

    pinMode(rst, OUTPUT);
    digitalWrite(rst, HIGH);
    digitalWrite(rst, LOW);
    digitalWrite(rst, HIGH);

    if (!rf95->init()) 
    {
        Serial.println("FAIL: Unable to init radio module");
        while(1);
    }

    if (!rf95->setFrequency(freq)) 
    {
        Serial.println("FAIL: Unable to set frequency");
        while(1);
    }
}

void Radio::reset(unsigned int duration)
{
    digitalWrite(reset_pin, LOW);
    delay(duration);
    digitalWrite(reset_pin, HIGH); 
    delay(duration);
}

uint8_t Radio::receive()
{
    if (rf95->available()) 
    {
        // NOTE! this is needed: it is treated as max length!
        // https://github.com/PaulStoffregen/RadioHead/blob/85a4df84c28e3cca2106bae020804b552337f8ea/RH_RF95.cpp#L223
        r_len = RH_RF95_MAX_MESSAGE_LEN;

        rf95->recv(receive_buffer, &r_len);

        return 1;
    }

    return 0;
}

uint8_t Radio::send(void *buffer, uint8_t size)
{
    return rf95->send((uint8_t *)buffer, size) && rf95->waitPacketSent();
}
