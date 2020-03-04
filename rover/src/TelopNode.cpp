#include "TelopNode.h"

void TelopNode::handle(joystick_t *data)
{
    dn.setDriveSpeed(data->y1, data->y2);
}

TelopNode ton;
