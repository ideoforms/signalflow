#include <soundio/soundio.h>

#include "output.h"

#include "../graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

signum::Graph *shared_graph = NULL;

namespace signum
{

void write_callback(struct SoundIoOutStream *outstream,
		int frame_count_min, int frame_count_max)
{
	const struct SoundIoChannelLayout *layout = &outstream->layout;
	struct SoundIoChannelArea *areas;
	int frame_count = frame_count_max;
	int frames_left = frame_count_max;

	/*-----------------------------------------------------------------------*
	 * On some drivers (eg Linux), we cannot write all samples at once.
	 * Keep writing as many as we can until we have cleared the buffer.
	 *-----------------------------------------------------------------------*/
	while (frames_left > 0)
	{
		int err;

		if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count)))
			throw std::runtime_error("libsoundio error on begin write: " + std::string(soundio_strerror(err)));

		if (!shared_graph)
			throw std::runtime_error("libsoundio error: No global graph created");

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

AudioOut::AudioOut(Graph *graph)
{
	this->graph = graph;
	shared_graph = graph;

	this->name = "audioout";
	this->channels_out = 2;
	this->init();
}

int AudioOut::init()
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

	this->device = soundio_get_output_device(this->soundio, default_out_device_index);
	if (!device)
		throw std::runtime_error("libsoundio init error: out of memory.");

	fprintf(stderr, "Output device: %s\n", device->name);

	this->outstream = soundio_outstream_create(device);
	this->outstream->format = SoundIoFormatFloat32NE;
	this->outstream->write_callback = write_callback;
	this->outstream->sample_rate = 44100.0;
	// this->outstream->software_latency = 512 / 44100.0;

	if ((err = soundio_outstream_open(this->outstream)))
		throw std::runtime_error("libsoundio init error: unable to open device: " + std::string(soundio_strerror(err)));

	if (this->outstream->layout_error)
		throw std::runtime_error("libsoundio init error: unable to set channel layout: " +
				std::string(soundio_strerror(this->outstream->layout_error)));

	if ((err = soundio_outstream_start(outstream)))
		throw std::runtime_error("libsoundio init error: unable to start device: " + std::string(soundio_strerror(err)));

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

void AudioOut::next(sample **out, int num_frames)
{
	for (int channel = 0; channel < this->channels_out; channel++)
		memset(out[channel], 0, num_frames * sizeof(sample));

	for (int index = 0; index < this->inputs.size(); index++)
	{
		UnitRef unit = this->inputs[index];
		for (int channel = 0; channel < this->channels_out; channel++)
		{
			#ifdef __APPLE__

			vDSP_vadd(unit->out[channel], 1, out[channel], 1, out[channel], 1, num_frames);

			#else

			for (int frame = 0; frame < num_frames; frame++)
			{
				out[channel][frame] += unit->out[channel][frame];
			}

			#endif
		}
	}
}

}

