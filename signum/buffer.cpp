#include <sndfile.h>
#include "buffer.h"

#include <stdlib.h>
#include <string.h>

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
		memset(this->data[channel], 0, sizeof(sample) * this->num_frames);
	}
}

Buffer::Buffer(const char *filename)
{
	this->open(filename);
}

void Buffer::open(const char *filename)
{
    SF_INFO info;
    SNDFILE *sndfile = sf_open(filename, SFM_READ, &info);

    if (!sndfile)
    {
        printf("Failed to read soundfile\n");
        exit(1);
    }

	printf("Read %d channels, %lld frames\n", info.channels, info.frames);
    this->data = (sample **) malloc(sizeof(void *) * info.channels);
	for (int channel = 0; channel < info.channels; channel++)
	{
		long long length = sizeof(sample) * info.frames;
		this->data[channel] = (sample *) malloc(length + 1024);
		memset(this->data[channel], 0, length);
	}

    int ptr = 0;
	int samples_per_read = 2048;

    while (true)
    {
        int count = sf_readf_float(sndfile, (float *) this->data[0] + ptr, samples_per_read);
        if (count < samples_per_read)
            break;

        ptr += count;
    }

	this->num_channels = info.channels;
	this->num_frames = info.frames;
	this->duration = this->num_frames / 44100.0;
}

}
