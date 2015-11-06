#import "buffer.h"

#import <stdlib.h>

namespace signum
{

Buffer::Buffer(int num_channels, int num_frames)
{
	this->num_channels = num_channels;
	this->num_frames = num_frames;

	this->data = (sample **) malloc(sizeof(void *) * this->num_channels);
	for (int channel = 0; channel < this->num_channels; channel++)
	{
		this->data[channel] = (sample *) malloc(sizeof(sample) * this->num_frames);
	}
}

}
