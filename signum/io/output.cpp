#include <soundio/soundio.h>

#include "output.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

namespace signum::io
{

static AudioOut *shared_out = NULL;

void write_callback(struct SoundIoOutStream *outstream,
        int frame_count_min, int frame_count_max)
{
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    struct SoundIoChannelArea *areas;
    int frame_count = frame_count_max;
	int err;

	if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count)))
	{
		fprintf(stderr, "%s\n", soundio_strerror(err));
		exit(1);
	}

	for (int frame = 0; frame < frame_count; frame++)
	{
		float sample = shared_out->next();
		for (int channel = 0; channel < layout->channel_count; channel += 1)
		{
			float *ptr = (float *)(areas[channel].ptr + areas[channel].step * frame);
			*ptr = sample;

			if (*ptr > 1.0) *ptr = 1.0;
			if (*ptr < -1.0) *ptr = -1.0;
		}
	}

	if ((err = soundio_outstream_end_write(outstream)))
	{
		fprintf(stderr, "%s\n", soundio_strerror(err));
		exit(1);
	}
}

AudioOut::AudioOut()
{
    int err;

	err = this->init();
	shared_out = this;
}

int AudioOut::init()
{
	int err;

    this->soundio = soundio_create();

    if (!this->soundio)
	{
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    if ((err = soundio_connect(this->soundio)))
	{
        fprintf(stderr, "error connecting: %s", soundio_strerror(err));
        return 1;
    }

    soundio_flush_events(this->soundio);

    int default_out_device_index = soundio_default_output_device_index(this->soundio);
    if (default_out_device_index < 0)
	{
        fprintf(stderr, "no output device found");
        return 1;
    }

    this->device = soundio_get_output_device(this->soundio, default_out_device_index);
    if (!device)
	{
        fprintf(stderr, "out of memory");
        return 1;
    }

    fprintf(stderr, "Output device: %s\n", device->name);

    this->outstream = soundio_outstream_create(device);
    outstream->format = SoundIoFormatFloat32NE;
    outstream->write_callback = write_callback;

    if ((err = soundio_outstream_open(outstream)))
	{
        fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
        return 1;
    }

    if (outstream->layout_error)
        fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

    if ((err = soundio_outstream_start(outstream)))
	{
        fprintf(stderr, "unable to start device: %s", soundio_strerror(err));
        return 1;
    }

    return 0;
}

int AudioOut::start()
{
    for (;;)
		soundio_wait_events(soundio);
}

int AudioOut::close()
{
    soundio_outstream_destroy(this->outstream);
    soundio_device_unref(this->device);
    soundio_destroy(this->soundio);

	return 0;
}

sample AudioOut::next()
{
	sample value = 0.0;

	for (int index = 0; index < this->inputs.size(); index++)
	{
		UnitRef unit = this->inputs[index];
		value += unit->next();
	}

	return value;
}

}
