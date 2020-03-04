#include "TelopNode.h"

void TelopNode::handle(joystick_t *data)
{
    dn.setDriveSpeed((int)255*data->y1, (int)255*data->y2);
}

TelopNode ton;
