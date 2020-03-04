#include "libradio.h"

namespace radio
{
	RH_RF95 *rf95;

	uint8_t recvBuffer[RH_RF95_MAX_MESSAGE_LEN];

	uint8_t r_len;

	uint8_t resetPin;

	void init(uint8_t rst, uint8_t cst, uint8_t intr, double freq)
	{
		resetPin = rst;

		rf95 = new RH_RF95(cst, intr);

		pinMode(rst, OUTPUT);
		digitalWrite(rst, HIGH);

		digitalWrite(rst, LOW);
		digitalWrite(rst, HIGH);

		Serial.print("Initializing radio... ");

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

	void reset(unsigned int duration)
	{
	    digitalWrite(resetPin, LOW);
	    delay(duration);
	    digitalWrite(resetPin, HIGH); 
	   	delay(duration);
	}

	uint8_t receive()
	{
		if (rf95->waitAvailableTimeout(1000)) 
		{
	        int packId;

	        rf95->recv(recvBuffer, &r_len);

	        return 1;
   		}

   		return 0;
	}

	uint8_t send(void *buffer, uint8_t size)
	{
		return rf95->send((uint8_t *)buffer, size) && rf95->waitPacketSent();
	}
}