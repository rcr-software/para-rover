#include "GatewayNode.h"

#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     6    // "D"
#define RF95_FREQ 915.0


void GatewayNode::init()
{
	radio::init(RFM95_RST, RFM95_CS, RFM95_INT, RF95_FREQ);
}

void GatewayNode::bg()
{
	int pacNo;

	if (radio::receive())
	{
        Serial.println("oh boy gotta packet!");
        Serial.println(radio::r_len);
		pacNo = *((int *)radio::recvBuffer);
        Serial.println(pacNo);

		switch (pacNo)
		{
			case PAC_JOYSTICK:
                Serial.println("It's a joystick!");
				ton.handle((joystick_t *)radio::recvBuffer); 
				break;
		}
	}
}

GatewayNode gwn;
