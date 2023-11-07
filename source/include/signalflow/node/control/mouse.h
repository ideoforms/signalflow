#pragma once

#include "signalflow/node/node.h"

#ifdef __APPLE__

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Outputs the normalised cursor X position, from 0 to 1.
 *---------------------------------------------------------------------------------*/
class MouseX : public Node
{
public:
    MouseX();
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Outputs the normalised cursor Y position, from 0 to 1.
 *---------------------------------------------------------------------------------*/
class MouseY : public Node
{
public:
    MouseY();
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Outputs 1 if the left mouse button is down, 0 otherwise.
 *---------------------------------------------------------------------------------*/
class MouseDown : public Node
{
public:
    MouseDown(NodeRef button_index = 0);
    virtual void process(Buffer &out, int num_frames);
    NodeRef button_index = 0;
};

REGISTER(MouseX, "mousex")
REGISTER(MouseY, "mousey")
REGISTER(MouseDown, "mousedown")
}

#endif /* __APPLE__ */
