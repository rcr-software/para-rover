#ifndef TELOPNODE_H_
#define TELOPNODE_H_

#include "main.h"
#include "packets.h"

class TelopNode
{
	private:

	public:
		void handle(joystick_t *data);
};

extern TelopNode ton;

#endif
