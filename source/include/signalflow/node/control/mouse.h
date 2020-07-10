#pragma once

#include "signalflow/node/node.h"

#ifdef __APPLE__

namespace signalflow
{
class MouseX : public Node
{
public:
    MouseX();
    virtual void process(sample **out, int num_frames);
};

class MouseY : public Node
{
public:
    MouseY();
    virtual void process(sample **out, int num_frames);
};

class MouseDown : public Node
{
public:
    MouseDown(NodeRef button_index = 0);
    virtual void process(sample **out, int num_frames);
    NodeRef button_index = 0;
};

REGISTER(MouseX, "mousex")
REGISTER(MouseY, "mousey")
REGISTER(MouseDown, "mousedown")
}

#endif /* __APPLE__ */
