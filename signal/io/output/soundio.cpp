#include "soundio.h"

#ifdef HAVE_SOUNDIO

#include "../../graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

namespace libsignal
{
    
extern Graph *shared_graph;
    
void write_callback(struct SoundIoOutStream *outstream,
		int frame_count_min, int frame_count_max)
{
	const struct SoundIoChannelLayout *layout = &outstream->layout;
	struct SoundIoChannelArea *areas;
	int frame_count = frame_count_max;
	int frames_left = frame_count_max;

	/*-----------------------------------------------------------------------*
	 * Return if the shared_graph hasn't been initialized yet.
	 * (The libsoundio Pulse Audio driver calls the write_callback once
	 * on initialization, so this may happen legitimately.)
	 *-----------------------------------------------------------------------*/
	if (!shared_graph || !shared_graph->output)
	{
		return;
	}

	/*-----------------------------------------------------------------------*
	 * On some drivers (eg Linux), we cannot write all samples at once.
	 * Keep writing as many as we can until we have cleared the buffer.
	 *-----------------------------------------------------------------------*/
	while (frames_left > 0)
	{
		int err;

		if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count)))
			throw std::runtime_error("libsoundio error on begin write: " + std::string(soundio_strerror(err)));

		shared_graph->pull_input(frame_count);

		for (int frame = 0; frame < frame_count; frame++)
		{
			for (int channel = 0; channel < layout->channel_count; channel += 1)
			{
				float *ptr = (float *)(areas[channel].ptr + areas[channel].step * frame);
				*ptr = shared_graph->output->out[channel][frame];

				/*-----------------------------------------------------------------------*
				 * Hard limiter.
				 *-----------------------------------------------------------------------*/
				if (*ptr > 1.0) *ptr = 1.0;
				if (*ptr < -1.0) *ptr = -1.0;
			}
		}

		if ((err = soundio_outstream_end_write(outstream)))
			throw std::runtime_error("libsoundio error on end write: " + std::string(soundio_strerror(err)));

		frames_left -= frame_count;
	}
}

int soundio_get_device_by_name(struct SoundIo *soundio, const char *name)
{
    int output_count = soundio_output_device_count(soundio);
    for (int i = 0; i < output_count; i++)
	{
        struct SoundIoDevice *device = soundio_get_output_device(soundio, i);
		if (strcmp(device->name, name) == 0)
		{
			printf("Found device %s: %d\n", name, i);
			return i;
		}
	}
	fprintf(stderr, "Couldn't find output device %s\n", name);

	return -1;
}


AudioOut_SoundIO::AudioOut_SoundIO(Graph *graph) : AudioOut_Abstract(graph)
{
    this->init();
}

int AudioOut_SoundIO::init()
{
	int err;

	this->soundio = soundio_create();

	if (!this->soundio)
		throw std::runtime_error("libsoundio init error: out of memory");

	if ((err = soundio_connect(this->soundio)))
		throw std::runtime_error("libsoundio init error: could not connect (" + std::string(soundio_strerror(err)) + ")");

	soundio_flush_events(this->soundio);

	int default_out_device_index = soundio_default_output_device_index(this->soundio);
	if (default_out_device_index < 0)
		throw std::runtime_error("libsoundio init error: no output devices found.");

	// int index = soundio_get_device_by_name(this->soundio, "Loopback Audio");
	// this->device = soundio_get_output_device(this->soundio, index);

	this->device = soundio_get_output_device(this->soundio, default_out_device_index);
	if (!device)
		throw std::runtime_error("libsoundio init error: out of memory.");

	this->outstream = soundio_outstream_create(device);
	this->outstream->format = SoundIoFormatFloat32NE;
	this->outstream->write_callback = write_callback;
	this->outstream->sample_rate = this->device->sample_rate_current;
	// this->outstream->software_latency = 1024 / 44100.0;

	this->sample_rate = this->outstream->sample_rate;

	fprintf(stderr, "Output device: %s (%dHz)\n", device->name, this->sample_rate);

	if ((err = soundio_outstream_open(this->outstream)))
		throw std::runtime_error("libsoundio init error: unable to open device: " + std::string(soundio_strerror(err)));

	if (this->outstream->layout_error)
		throw std::runtime_error("libsoundio init error: unable to set channel layout: " +
				std::string(soundio_strerror(this->outstream->layout_error)));

	if ((err = soundio_outstream_start(outstream)))
		throw std::runtime_error("libsoundio init error: unable to start device: " + std::string(soundio_strerror(err)));

	return 0;
}

int AudioOut_SoundIO::start()
{
	for (;;)
		soundio_wait_events(soundio);
}

int AudioOut_SoundIO::close()
{
	soundio_outstream_destroy(this->outstream);
	soundio_device_unref(this->device);
	soundio_destroy(this->soundio);

	return 0;
}

} // namespace libsignal

#endif

