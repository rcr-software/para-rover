#include "GatewayNode.h"

#define RFM95_RST     4   // "A"
#define RFM95_CS      8   // "B"
#define RFM95_INT     3    // "D"
#define RF95_FREQ 915.0


void GatewayNode::init()
{
	radio::init(RFM95_RST, RFM95_CS, RFM95_INT, RF95_FREQ);
}

void GatewayNode::bg()
{
	uint8_t pacNo;

	if (radio::receive())
	{
		pacNo = *((uint8_t *)radio::recvBuffer);

		switch (pacNo)
		{
			case PAC_JOYSTICK:
				ton.handle((joystick_t *)radio::recvBuffer); //or similar
				break;
		}
	}
}

GatewayNode gwn;
