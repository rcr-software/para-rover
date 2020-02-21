#include TELOPNODE_H_
#define TELOPNODE_H_

#include "main.h"

class TelopNode
{
	private:

	public:

		void handle(joystick_t *data)
		{
			dn.setDriveSpeed(data.y1, data.y2);
		}

}

#endif