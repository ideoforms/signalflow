#include "sampler.h"

#include <sndfile.h>
#include <stdlib.h>

namespace signum::gen
{

Sampler::Sampler(const char *filename, float rate, bool loop)
{
	this->rate = rate;
	this->loop = loop;

    SF_INFO info;
    SNDFILE *sndfile = sf_open(filename, SFM_READ, &info);

	if (!sndfile)
	{
		printf("Failed to read soundfile\n");
		exit(1);
	}

    this->buffer = (float *) malloc(sizeof(double) * info.frames);
	// printf("Sample read: %p, frames = %lld\n", sndfile, info.frames);
    int ptr = 0;

    while (true)
    {
        int count = sf_readf_float(sndfile, this->buffer + ptr, 2048);
        if (count == 0)
            break;

		ptr += count;
    }

	this->phase = 0;
	this->num_frames = info.frames;
}

void Sampler::next(int count)
{
	for (int i = 0; i < count; i++)
	{
		sample s;
		if ((int) this->phase < this->num_frames)
		{
			s = this->buffer[(int) this->phase];
		}
		else
		{
			if (loop)
			{
				this->phase = 0;
				s = this->buffer[(int) this->phase];
			}
			else
			{
				s = 0.0;
			}
		}

		this->output->data[0][i] = s;
		this->phase += this->rate;
	}
}

void Sampler::trigger()
{
	this->phase = 0;
}

}
