#include "libradio.h"

namespace radio
{
	RH_RF95 *rf95;

	uint8_t resetPin;

	void init(uint8_t rst, uint8_t cst, uint8_t intr, unsigned double freq)
	{
		resetPin = rst;

		rf95 = new RH_RF95(cst, intr);

		pinMode(rst, output);
		digitalWrite(rst, HIGH);

		digitalWrite(rst, LOW);
		digitalWrite(rst, HIGH);

		Serial.print("Initializing radio... ");

	    if (!rf95.init()) 
	    {
	    	Serial.println("FAIL: Unable to init radio module");
	        while(1);
	    }

	    if (!rf95.setFrequency(RF95_FREQ)) 
	    {
	    	Serial.println("FAIL: Unable to set frequency")
	        while(1);
	    }
	}

	void reset(unsigned int duration)
	{
	    digitalWrite(RFM95_RST, LOW);
	    delay(duration);
	    digitalWrite(RFM95_RST, HIGH); 
	   	delay(duration);
	}
}