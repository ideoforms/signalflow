#include "signalflow/node/io/output/soundio.h"

#ifdef HAVE_SOUNDIO

#include "signalflow/core/graph.h"

#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_processing = false;

namespace signalflow
{

extern AudioGraph *shared_graph;

void write_callback(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max)
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
            throw audio_io_exception("libsoundio error on begin write: " + std::string(soundio_strerror(err)));
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

            NodeRef output = shared_graph->get_output();
            for (int frame = 0; frame < frame_count; frame++)
            {
                for (int channel = 0; channel < layout->channel_count; channel += 1)
                {
                    if (outstream->format == SoundIoFormatFloat32NE)
                    {
                        float *ptr = reinterpret_cast<float *>(areas[channel].ptr + areas[channel].step * frame);
                        *ptr = output->out[channel][frame];
                        /*-----------------------------------------------------------------------*
                         * Hard limiter.
                         *-----------------------------------------------------------------------*/
                        if (*ptr > 1.0)
                            *ptr = 1.0;
                        if (*ptr < -1.0)
                            *ptr = -1.0;
                    }
                    else if (outstream->format == SoundIoFormatS16LE)
                    {
                        int16_t *ptr = reinterpret_cast<int16_t *>(areas[channel].ptr + areas[channel].step * frame);
                        *ptr = (int16_t)(output->out[channel][frame] * 32768.0f);
                    }
                }
            }
        }
        else
        {
            for (int frame = 0; frame < frame_count; frame++)
            {
                for (int channel = 0; channel < layout->channel_count; channel += 1)
                {
                    float *ptr = reinterpret_cast<float *>(areas[channel].ptr + areas[channel].step * frame);
                    *ptr = 0;
                }
            }
        }

        if ((err = soundio_outstream_end_write(outstream)))
        {
            throw audio_io_exception("libsoundio error on end write: " + std::string(soundio_strerror(err)));
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
    std::cerr << "Couldn't find output device " << std::string(name) << std::endl;

    return -1;
}

AudioOut_SoundIO::AudioOut_SoundIO(const std::string &backend_name,
                                   const std::string &device_name,
                                   unsigned int sample_rate,
                                   unsigned int buffer_size)
    : AudioOut_Abstract()
{
    this->backend_name = backend_name;
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
        throw audio_io_exception("libsoundio error: out of memory");

    if (!this->backend_name.empty())
    {
        // Backend name is specified; connect to the given backend
        std::vector<std::string> possible_backend_names = {
            "none",
            "jack",
            "pulseaudio",
            "alsa",
            "coreaudio",
            "wasapi",
            "dummy"
        };

        auto location = std::find(possible_backend_names.begin(),
                                  possible_backend_names.end(),
                                  this->backend_name);

        if (location == possible_backend_names.end())
        {
            throw audio_io_exception("libsoundio error: could not find backend name " + this->backend_name);
        }
        enum SoundIoBackend backend_index = (enum SoundIoBackend)(location - possible_backend_names.begin());
        if ((err = soundio_connect_backend(this->soundio, backend_index)))
            throw audio_io_exception("libsoundio error: could not connect (" + std::string(soundio_strerror(err)) + ")");
    }
    else
    {
        if ((err = soundio_connect(this->soundio)))
            throw audio_io_exception("libsoundio error: could not connect (" + std::string(soundio_strerror(err)) + ")");
    }

    soundio_flush_events(this->soundio);

    int default_out_device_index = soundio_default_output_device_index(this->soundio);
    if (default_out_device_index < 0)
        throw device_not_found_exception("No audio devices were found. More information: https://signalflow.dev/troubleshooting/device_not_found_exception/");

    if (!this->device_name.empty())
    {
        int index = soundio_get_device_by_name(this->soundio, this->device_name.c_str());
        if (index == -1)
        {
            throw device_not_found_exception("Could not find device name: " + this->device_name + ".  More information: https://signalflow.dev/troubleshooting/device_not_found_exception/");
        }
        this->device = soundio_get_output_device(this->soundio, index);
    }
    else
    {
        this->device = soundio_get_output_device(this->soundio, default_out_device_index);
    }

    if (!device)
        throw audio_io_exception("libsoundio error: out of memory.");

    this->outstream = soundio_outstream_create(device);
    if (soundio_device_supports_format(device, SoundIoFormatFloat32NE))
    {
        this->outstream->format = SoundIoFormatFloat32NE;
    }
    else if (soundio_device_supports_format(device, SoundIoFormatS16LE))
    {
        this->outstream->format = SoundIoFormatS16LE;
    }
    else
    {
        /*-----------------------------------------------------------------------*
         * SignalFlow currently only supports float32 sample output
         *-----------------------------------------------------------------------*/
        throw audio_io_exception("libsoundio error: Output device does not support float32 or int16le samples");
    }
    this->outstream->write_callback = write_callback;
    if (!this->sample_rate)
    {
        this->sample_rate = this->device->sample_rate_current;
    }
    this->outstream->sample_rate = this->sample_rate;
    this->outstream->software_latency = (double) this->buffer_size / this->outstream->sample_rate;
    this->outstream->userdata = (void *) this;
    // With a device with multiple layouts, use the first.
    // To check: is this always the layout with the most channels?
    this->outstream->layout = device->layouts[0];

    if ((err = soundio_outstream_open(this->outstream)))
    {
        throw audio_io_exception("libsoundio error: unable to open device: " + std::string(soundio_strerror(err)));
    }

    if (this->outstream->layout_error)
    {
        /*--------------------------------------------------------------------------------
         * This should not be a fatal error (see example in libsoundio sio_sine.c).
         * Should just generate a warning instead.
         * Experienced on Raspberry Pi 4 with raspi-audio interface.
         *-------------------------------------------------------------------------------*/
        std::cerr << "libsoundio warning: unable to set channel layout: "
                  << std::string(soundio_strerror(this->outstream->layout_error)) << std::endl;
    }

    this->num_output_channels = this->outstream->layout.channel_count;

    // update based on the actual buffer size
    this->buffer_size = (int) round(this->outstream->software_latency * this->outstream->sample_rate);

    std::string s = num_output_channels == 1 ? "" : "s";

    std::cerr << "[soundio] Output device: " << device->name << " (" << sample_rate << "Hz, "
              << "buffer size " << buffer_size << " samples, " << num_output_channels << " channel" << s << ")"
              << std::endl;

    // do we need to set num_output channels to allocate the right number of output buffers?
    this->set_channels(num_output_channels, 0);

    return 0;
}

int AudioOut_SoundIO::start()
{
    int err;
    if ((err = soundio_outstream_start(outstream)))
        throw audio_io_exception("libsoundio error: unable to start device: " + std::string(soundio_strerror(err)));
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

std::list<std::string> AudioOut_SoundIO::get_output_device_names()
{
    int output_count = soundio_output_device_count(this->soundio);

    std::list<std::string> device_names;

    for (int i = 0; i < output_count; i++)
    {
        struct SoundIoDevice *device = soundio_get_output_device(soundio, i);
        device_names.push_back(std::string(device->name));
    }

    return device_names;
}

int AudioOut_SoundIO::get_default_output_device_index()
{
    unsigned int default_output = soundio_default_output_device_index(this->soundio);
    return default_output;
}

std::list<std::string> AudioOut_SoundIO::get_output_backend_names()
{
    std::list<std::string> backend_names;
    std::vector<std::string> possible_backend_names = {
        "none",
        "jack",
        "pulseaudio",
        "alsa",
        "coreaudio",
        "wasapi",
        "dummy"
    };
    for (int i = 0; i < soundio_backend_count(this->soundio); i++)
    {
        int backend_index = soundio_get_backend(this->soundio, i);
        std::string backend_name = possible_backend_names[backend_index];
        backend_names.push_back(backend_name);
    }

    return backend_names;
}

} // namespace signalflow

#endif
