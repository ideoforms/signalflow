#include "signalflow/node/io/input/soundio.h"

#ifdef HAVE_SOUNDIO

#define SIGNALFLOW_AUDIO_IN_DEFAULT_BUFFER_SIZE 1024

#include "signalflow/core/graph.h"
#include "signalflow/node/io/output/soundio.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_processing = false;

namespace signalflow
{
extern AudioIn_Abstract *shared_in;

void read_callback(struct SoundIoInStream *instream,
                   int frame_count_min, int frame_count_max)
{
    is_processing = true;

    AudioIn_SoundIO *input = (AudioIn_SoundIO *) shared_in;
    if (!shared_in)
        return;

    const struct SoundIoChannelLayout *layout = &instream->layout;
    struct SoundIoChannelArea *areas;
    int frame_count = frame_count_max;
    int frames_left = frame_count_max;

    /*-----------------------------------------------------------------------*
     * On some drivers (eg Linux), we cannot write all samples at once.
     * Keep reading as many as we can until we have cleared the buffer.
     *-----------------------------------------------------------------------*/
    while (frames_left > 0)
    {
        int err;

        if ((err = soundio_instream_begin_read(instream, &areas, &frame_count)))
            throw audio_io_exception("libsoundio error on begin read: " + std::string(soundio_strerror(err)));

        if (!input)
            continue;
        // throw std::runtime_error("libsoundio error: No global input created");

        for (int frame = 0; frame < frame_count; frame++)
        {
            for (int channel = 0; channel < input->buffer->get_num_channels(); channel += 1)
            {
                float *ptr = reinterpret_cast<float *>(areas[channel].ptr + areas[channel].step * frame);
                input->buffer->data[channel][input->write_pos] = *ptr;
            }
            input->write_pos = (input->write_pos + 1) % input->buffer->get_num_frames();
        }

        if ((err = soundio_instream_end_read(instream)))
            throw audio_io_exception("libsoundio error on end read: " + std::string(soundio_strerror(err)));

        frames_left -= frame_count;
    }

    is_processing = false;
}

AudioIn_SoundIO::AudioIn_SoundIO(unsigned int num_channels)
    : AudioIn_Abstract()
{
    // Allocate enough buffer for twice our block size, else
    // we risk overwriting our input buffer from the audio in
    // while it is still being read from.
    // TODO: Bad hardcoded block size

    this->num_channels_requested = num_channels;
    this->read_pos = 0;
    this->write_pos = (int) (SIGNALFLOW_AUDIO_IN_DEFAULT_BUFFER_SIZE / 2);
    this->name = "audioin_soundio";
    this->buffer = NULL;

    this->init();
}

AudioIn_SoundIO::~AudioIn_SoundIO()
{
    this->destroy();
}

int AudioIn_SoundIO::init()
{
    int err;

    this->soundio = ((AudioOut_SoundIO *) this->graph->get_output().get())->soundio;

    if (!this->soundio)
        throw audio_io_exception("libsoundio init error: No output node found in graph (initialising input before output?)");

    int default_in_device_index = soundio_default_input_device_index(this->soundio);
    if (default_in_device_index < 0)
        throw device_not_found_exception("No input devices found.");

    this->device = soundio_get_input_device(this->soundio, default_in_device_index);
    if (!device)
        throw audio_io_exception("libsoundio init error: out of memory.");

    this->instream = soundio_instream_create(device);
    this->instream->format = SoundIoFormatFloat32NE;
    this->instream->read_callback = read_callback;
    this->instream->sample_rate = device->sample_rate_current;
    this->instream->software_latency = 256.0 / this->instream->sample_rate;

    if ((err = soundio_instream_open(this->instream)))
    {
        throw audio_io_exception("libsoundio init error: unable to open device: " + std::string(soundio_strerror(err)));
    }

    if ((err = soundio_instream_start(instream)))
    {
        throw audio_io_exception("libsoundio init error: unable to start device: " + std::string(soundio_strerror(err)));
    }

    if (this->num_channels_requested > this->instream->layout.channel_count)
    {
        throw audio_io_exception("AudioIn: Not enough input channels available (requested " + std::to_string(this->num_channels_requested) + ", available " + std::to_string(this->instream->layout.channel_count) + ")");
    }
    this->set_channels(0, this->num_channels_requested);
    this->buffer = new Buffer(this->num_output_channels, SIGNALFLOW_AUDIO_IN_DEFAULT_BUFFER_SIZE);

    int buffer_size = this->instream->software_latency * this->instream->sample_rate;
    std::string s = num_output_channels == 1 ? "" : "s";

    std::cerr << "Input device: " << device->name << " (" << this->instream->sample_rate << "Hz, "
              << "buffer size " << buffer_size << " samples, " << num_output_channels << " channel" << s << ")" << std::endl;

    return 0;
}

int AudioIn_SoundIO::start()
{
    return 0;
}

int AudioIn_SoundIO::stop()
{
    return 0;
}

int AudioIn_SoundIO::destroy()
{
    while (is_processing)
    {
    }

    shared_in = nullptr;
    soundio_instream_destroy(this->instream);
    soundio_device_unref(this->device);

    return 0;
}

void AudioIn_SoundIO::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < num_output_channels; channel++)
        {
            out[channel][frame] = this->buffer->data[channel][this->read_pos];
        }
        this->read_pos = (this->read_pos + 1) % this->buffer->get_num_frames();
    }
}

}

#endif
