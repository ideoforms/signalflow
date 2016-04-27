#include "mouse.h"

#ifdef __APPLE__

#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>

namespace libsignal
{

MouseX::MouseX() : Node()
{
	this->name = "mousex";
}

void MouseX::process(sample **out, int num_frames)
{
	NSPoint mouseLoc = [NSEvent mouseLocation];
	NSRect screenRect = [[NSScreen mainScreen] frame];
	float x = (float) mouseLoc.x / screenRect.size.width;

	for (int frame = 0; frame < num_frames; frame++)
		for (int channel = 0; channel < this->num_output_channels; channel++)
			out[channel][frame] = x;
}

MouseY::MouseY() : Node()
{
	this->name = "mousey";
}

void MouseY::process(sample **out, int num_frames)
{
	NSPoint mouseLoc = [NSEvent mouseLocation];
	NSRect screenRect = [[NSScreen mainScreen] frame];
	float y = (float) mouseLoc.y / screenRect.size.height;

	for (int frame = 0; frame < num_frames; frame++)
		for (int channel = 0; channel < this->num_output_channels; channel++)
			out[channel][frame] = y;
}

}

#endif /* __APPLE__ */
