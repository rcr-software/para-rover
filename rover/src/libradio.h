#ifndef LIBRADIO_H_
#define LIBRADIO_H_

#include <RH_RF95.h>



namespace radio
{
	extern RH_RF95 *rf95;

	extern uint8_t recvBuffer[RH_RF95_MAX_MESSAGE_LEN];

	extern uint8_t r_len;

	extern uint8_t resetPin;
	
	void init(uint8_t rst, uint8_t cst, uint8_t intr, double freq);

	void reset(unsigned int duration);

	uint8_t receive();

	uint8_t send(void *buffer, uint8_t size);
}

#endif