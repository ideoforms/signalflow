#include "signalflow/node/io/output/soundio.h"

#ifdef HAVE_SOUNDIO

#include "signalflow/core/graph.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_processing = false;

namespace signalflow
{

extern AudioGraph *shared_graph;

void write_callback(struct SoundIoOutStream *outstream,
                    int frame_count_min,
                    int frame_count_max)
{
    is_processing = true;

    const struct SoundIoChannelLayout *layout = &outstream->layout;
    struct SoundIoChannelArea *areas;
    int frame_count = frame_count_max;
    int frames_left = frame_count_max;

    /*-----------------------------------------------------------------------*
     * Return if the shared_graph hasn't been initialized yet.
     * (The libsoundio Pulse Audio driver calls the write_callback once
     * on initialization, so this may happen legitimately.)
     *-----------------------------------------------------------------------*/
    if (!shared_graph || !shared_graph->get_output())
    {
        return;
    }

    AudioOut_SoundIO *out_node = (AudioOut_SoundIO *) outstream->userdata;

    /*-----------------------------------------------------------------------*
     * On some drivers (eg Linux), we cannot write all samples at once.
     * Keep writing as many as we can until we have cleared the buffer.
     *-----------------------------------------------------------------------*/
    while (frames_left > 0)
    {
        int err;

        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count)))
        {
            throw std::runtime_error("libsoundio error on begin write: " + std::string(soundio_strerror(err)));
        }
        if (out_node->get_state() == SIGNALFLOW_NODE_STATE_ACTIVE)
        {
            try
            {
                shared_graph->render(frame_count);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Exception in AudioGraph: " << e.what() << std::endl;
                exit(1);
            }

            for (int frame = 0; frame < frame_count; frame++)
            {
                for (int channel = 0; channel < layout->channel_count; channel += 1)
                {
                    float *ptr = (float *) (areas[channel].ptr + areas[channel].step * frame);
                    *ptr = shared_graph->get_output()->out[channel][frame];

                    /*-----------------------------------------------------------------------*
                     * Hard limiter.
                     *-----------------------------------------------------------------------*/
                    if (*ptr > 1.0)
                        *ptr = 1.0;
                    if (*ptr < -1.0)
                        *ptr = -1.0;
                }
            }
        }
        else
        {
            for (int frame = 0; frame < frame_count; frame++)
            {
                for (int channel = 0; channel < layout->channel_count; channel += 1)
                {
                    float *ptr = (float *) (areas[channel].ptr + areas[channel].step * frame);
                    *ptr = 0;
                }
            }
        }

        if ((err = soundio_outstream_end_write(outstream)))
        {
            throw std::runtime_error("libsoundio error on end write: " + std::string(soundio_strerror(err)));
        }

        frames_left -= frame_count;
    }

    is_processing = false;
}

int soundio_get_device_by_name(struct SoundIo *soundio, const char *name)
{
    int output_count = soundio_output_device_count(soundio);
    for (int i = 0; i < output_count; i++)
    {
        struct SoundIoDevice *device = soundio_get_output_device(soundio, i);
        if (strcmp(device->name, name) == 0)
        {
            return i;
        }
    }
    std::cerr << "Couldn't find output device" << std::string(name) << std::endl;

    return -1;
}

AudioOut_SoundIO::AudioOut_SoundIO(const std::string &device_name,
                                   unsigned int sample_rate,
                                   unsigned int buffer_size)
    : AudioOut_Abstract()
{
    this->device_name = device_name;
    this->sample_rate = sample_rate;
    this->buffer_size = buffer_size;
    this->name = "audioout-soundio";

    this->init();
}

int AudioOut_SoundIO::init()
{
    int err;

    this->soundio = soundio_create();

    if (!this->soundio)
        throw std::runtime_error("libsoundio error: out of memory");

    if ((err = soundio_connect(this->soundio)))
        throw std::runtime_error("libsoundio error: could not connect (" + std::string(soundio_strerror(err)) + ")");

    soundio_flush_events(this->soundio);

    int default_out_device_index = soundio_default_output_device_index(this->soundio);
    if (default_out_device_index < 0)
        throw std::runtime_error("libsoundio error: no output devices found.");

    if (!this->device_name.empty())
    {
        int index = soundio_get_device_by_name(this->soundio, this->device_name.c_str());
        if (index == -1)
        {
            throw std::runtime_error("Could not find device name: " + this->device_name);
        }
        this->device = soundio_get_output_device(this->soundio, index);
    }
    else
    {
        this->device = soundio_get_output_device(this->soundio, default_out_device_index);
    }

    if (!device)
        throw std::runtime_error("libsoundio error: out of memory.");

    this->outstream = soundio_outstream_create(device);
    if (soundio_device_supports_format(device, SoundIoFormatFloat32NE))
    {
        this->outstream->format = SoundIoFormatFloat32NE;
    }
    else
    {
        throw std::runtime_error("libsoundio error: sample format not supported");
    }
    this->outstream->write_callback = write_callback;
    this->outstream->sample_rate = this->device->sample_rate_current;
    this->outstream->software_latency = (double) this->buffer_size / this->outstream->sample_rate;
    this->outstream->userdata = (void *) this;

    this->sample_rate = this->outstream->sample_rate;

    if ((err = soundio_outstream_open(this->outstream)))
    {
        throw std::runtime_error("libsoundio error: unable to open device: " + std::string(soundio_strerror(err)));
    }

    if (this->outstream->layout_error)
    {
        /*--------------------------------------------------------------------------------
         * This should not be a fatal error (see example in libsoundio sio_sine.c).
         * Should just generate a warning instead.
         * Experienced on Raspberry Pi 4 with raspi-audio interface.
         *-------------------------------------------------------------------------------*/
        std::cerr << "libsoundio warning: unable to set channel layout: " << std::string(soundio_strerror(this->outstream->layout_error)) << std::endl;
    }

    this->num_output_channels = this->outstream->layout.channel_count;

    // update based on the actual buffer size
    this->buffer_size = (int) round(this->outstream->software_latency * this->outstream->sample_rate);

    std::string s = num_output_channels == 1 ? "" : "s";

    std::cerr << "Output device: " << device->name << " (" << sample_rate << "Hz, "
              << "buffer size " << buffer_size << " samples, " << num_output_channels << " channel" << s << ")" << std::endl;

    // do we need to set num_output channels to allocate the right number of output buffers?
    this->set_channels(num_output_channels, 0);

    return 0;
}

int AudioOut_SoundIO::start()
{
    int err;
    if ((err = soundio_outstream_start(outstream)))
        throw std::runtime_error("libsoundio error: unable to start device: " + std::string(soundio_strerror(err)));
    this->set_state(SIGNALFLOW_NODE_STATE_ACTIVE);

    return 0;
}

int AudioOut_SoundIO::stop()
{
    this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
    return 0;
}

int AudioOut_SoundIO::destroy()
{
    while (is_processing)
    {
    }

    soundio_outstream_destroy(this->outstream);
    soundio_device_unref(this->device);
    soundio_destroy(this->soundio);

    return 0;
}

} // namespace signalflow

#endif
