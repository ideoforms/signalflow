#include "signalflow/node/io/input/miniaudio.h"

#include "signalflow/core/graph.h"
#include "signalflow/node/io/output/miniaudio.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

static bool is_processing = false;

namespace signalflow
{

AudioIn *shared_in;

void read_callback(ma_device *pDevice,
                   void *pOutput,
                   const void *pInput,
                   ma_uint32 frameCount)
{
    is_processing = true;

    AudioIn *input_node = (AudioIn *) shared_in;
    if (!input_node)
        return;

    float *input_samples = (float *) pInput;

    // TODO: the number of channels at the mic input might not be the same as the number of channels of this device
    int num_channels = input_node->get_num_output_channels();
    for (unsigned int frame = 0; frame < frameCount; frame++)
    {
        for (int channel = 0; channel < num_channels; channel++)
        {
            input_node->out[channel][frame] = input_samples[frame * num_channels + channel];
        }
    }

    is_processing = false;
}

AudioIn::AudioIn(unsigned int num_channels)
    : AudioIn_Abstract()
{
    shared_in = this;
    this->name = "audioin-miniaudio";
    this->num_channels = num_channels;
    this->init();
}

AudioIn::~AudioIn()
{
    // TODO: call superclass destructor to set shared_in to null
    this->destroy();
}

void AudioIn::init()
{
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format = ma_format_f32;
    config.capture.channels = this->num_channels;
    config.periodSizeInFrames = this->get_graph()->get_output_buffer_size();
    config.sampleRate = this->get_graph()->get_sample_rate();
    config.dataCallback = read_callback;

    ma_result rv = ma_device_init(NULL, &config, &device);
    if (rv != MA_SUCCESS)
    {
        throw audio_io_exception("miniaudio: Error initialising input device");
    }

    this->set_channels(0, device.capture.internalChannels);

    /*--------------------------------------------------------------------------------
     * Note that the underlying sample rate used by the recording hardware
     * (`device.capture.internalSampleRate`) may not be the same as that used
     * by `AudioIn`: SignalFlow requires that the input and output streams are both
     * on the same sample rate, so miniaudio's resampling is used to unify them.
     *-------------------------------------------------------------------------------*/
    std::string s = device.capture.internalChannels == 1 ? "" : "s";
    std::cerr << "[miniaudio] Input device: " << std::string(device.capture.name) << " (" << device.capture.internalSampleRate << "Hz, "
              << "buffer size " << device.capture.internalPeriodSizeInFrames << " samples, " << device.capture.internalChannels << " channel" << s << ")"
              << std::endl;

    this->start();
}

void AudioIn::start()
{
    ma_result rv = ma_device_start(&device);
    if (rv != MA_SUCCESS)
    {
        throw audio_io_exception("miniaudio: Error starting device");
    }
}

void AudioIn::stop()
{
    ma_result rv = ma_device_stop(&device);
    if (rv != MA_SUCCESS)
    {
        throw audio_io_exception("miniaudio: Error stopping device");
    }
}

void AudioIn::destroy()
{
    while (is_processing)
    {
    }

    this->stop();
    shared_in = nullptr;
}

void AudioIn::process(Buffer &out, int num_samples)
{
}

}
