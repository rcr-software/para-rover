#ifndef GATEWAYNODE_H_
#define GATEWAYNODE_H_

#include "libradio.h"
#include "packets.h"

#define RFM95_RST     4   // "A"
#define RFM95_CS      8   // "B"
#define RFM95_INT     3    // "D"
#define RF95_FREQ 915.0

class GatewayNode
{
	private:

	public:
		void init();

		void bg();
}


#endif