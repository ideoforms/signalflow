#include "signalflow/node/io/input/miniaudio.h"

#include "signalflow/core/graph.h"
#include "signalflow/node/io/output/miniaudio.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

static bool is_processing = false;

namespace signalflow
{
extern AudioIn_Abstract *shared_in;

void read_callback(ma_device *pDevice,
                   void *pOutput,
                   const void *pInput,
                   ma_uint32 frameCount)
{
    is_processing = true;

    AudioIn_MiniAudio *input_node = (AudioIn_MiniAudio *) shared_in;
    if (!input_node)
        return;

    float *input_samples = (float *) pInput;

    // TODO: the number of channels at the mic input might not be the same as the number of channels of this device
    int num_channels = input_node->get_num_output_channels();
    for (int frame = 0; frame < frameCount; frame++)
    {
        for (int channel = 0; channel < num_channels; channel++)
        {
            input_node->out[channel][frame] = input_samples[frame * num_channels + channel];
        }
    }

    is_processing = false;
}

AudioIn_MiniAudio::AudioIn_MiniAudio(unsigned int num_channels)
    : AudioIn_Abstract()
{
    this->name = "audioin-miniaudio";
    this->num_channels = num_channels;
    this->init();
}

AudioIn_MiniAudio::~AudioIn_MiniAudio()
{
    // TODO: call superclass destructor to set shared_in to null
    this->destroy();
}

int AudioIn_MiniAudio::init()
{
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format = ma_format_f32;
    config.capture.channels = this->num_channels;
    config.periodSizeInFrames = 0;
    config.sampleRate = this->get_graph()->get_sample_rate();
    config.dataCallback = read_callback;

    ma_result rv = ma_device_init(NULL, &config, &device);
    if (rv != MA_SUCCESS)
    {
        throw std::runtime_error("miniaudio: Error initialising input device");
    }

    this->set_channels(0, device.capture.internalChannels);

    std::string s = device.capture.internalChannels == 1 ? "" : "s";
    std::cerr << "[miniaudio] Input device: " << std::string(device.capture.name) << " (" << device.capture.internalSampleRate << "Hz, "
              << "buffer size " << device.capture.internalPeriodSizeInFrames << " samples, " << device.capture.internalChannels << " channel" << s << ")"
              << std::endl;

    this->start();
}

int AudioIn_MiniAudio::start()
{
    ma_result rv = ma_device_start(&device);
    if (rv != MA_SUCCESS)
    {
        throw std::runtime_error("miniaudio: Error starting device");
    }
}

int AudioIn_MiniAudio::stop()
{
    ma_result rv = ma_device_stop(&device);
    if (rv != MA_SUCCESS)
    {
        throw std::runtime_error("miniaudio: Error stopping device");
    }
}

int AudioIn_MiniAudio::destroy()
{
    while (is_processing)
    {
    }

    this->stop();
    shared_in = nullptr;

    return 0;
}

void AudioIn_MiniAudio::process(Buffer &out, int num_samples)
{
}

}
