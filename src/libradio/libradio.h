#ifndef LIBRADIO_H_
#define LIBRADIO_H_

#include <RH_RF95.h>

namespace radio
{
	RH_RF95 *rf95;

	uint8_t resetPin;
	
	void init(uint8_t rst, uint8_t cst, uint8_t intr, unsigned double freq);

	void reset(unsigned int duration);
}

#endif