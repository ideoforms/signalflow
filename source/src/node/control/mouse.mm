#include "signalflow/node/control/mouse.h"

#ifdef __APPLE__

#include <AppKit/AppKit.h>
#include <Foundation/Foundation.h>

namespace signalflow
{

MouseX::MouseX()
    : Node()
{
    this->name = "mousex";

    /*--------------------------------------------------------------------------------
     * The first time the mouse location is polled, it appears to do some additional
     * initialisation that is non-real-time safe and causes a buffer overrun in
     * the real-time thread. Do it here instead.
     *--------------------------------------------------------------------------------*/
    [NSEvent mouseLocation];
}

void MouseX::process(Buffer &out, int num_frames)
{
    NSPoint mouseLoc = [NSEvent mouseLocation];
    NSRect screenRect = [[NSScreen mainScreen] frame];
    float x = (float)mouseLoc.x / screenRect.size.width;

    for (int frame = 0; frame < num_frames; frame++)
        for (int channel = 0; channel < this->num_output_channels; channel++)
            out[channel][frame] = x;
}

MouseY::MouseY()
    : Node()
{
    this->name = "mousey";

    /*--------------------------------------------------------------------------------
     * See comment in MouseX
     *--------------------------------------------------------------------------------*/
    [NSEvent mouseLocation];
}

void MouseY::process(Buffer &out, int num_frames)
{
    NSPoint mouseLoc = [NSEvent mouseLocation];
    NSRect screenRect = [[NSScreen mainScreen] frame];
    float y = (float)mouseLoc.y / screenRect.size.height;

    for (int frame = 0; frame < num_frames; frame++)
        for (int channel = 0; channel < this->num_output_channels; channel++)
            out[channel][frame] = y;
}

MouseDown::MouseDown(NodeRef button_index)
    : button_index(button_index)
{
    this->name = "mousedown";
    this->create_input("button_index", this->button_index);

    /*--------------------------------------------------------------------------------
     * See comment in MouseX
     *--------------------------------------------------------------------------------*/
    [NSEvent pressedMouseButtons];
}

void MouseDown::process(Buffer &out, int num_frames)
{
    NSUInteger pressedMouseButtons = [NSEvent pressedMouseButtons];
    float rv = 0.0;
    if ((int)pressedMouseButtons & (1 << (int)this->button_index->out[0][0]))
    {
        rv = 1.0;
    }

    for (int frame = 0; frame < num_frames; frame++)
        for (int channel = 0; channel < this->num_output_channels; channel++)
            out[channel][frame] = rv;
}

}

#endif /* __APPLE__ */
