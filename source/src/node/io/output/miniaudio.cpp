#include "signalflow/node/io/output/miniaudio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "signalflow/node/io/output/miniaudio-library.h"

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

void data_callback(ma_device *pDevice,
                   void *pOutput,
                   const void *pInput,
                   ma_uint32 frame_count)
{
    is_processing = true;
    int channel_count = pDevice->playback.channels;

    /*-----------------------------------------------------------------------*
     * Return if the shared_graph hasn't been initialized yet.
     * (The libsoundio Pulse Audio driver calls the write_callback once
     * on initialization, so this may happen legitimately.)
     *-----------------------------------------------------------------------*/
    if (!shared_graph || !shared_graph->get_output())
    {
        return;
    }

    float *output_pointer = (float *) pOutput;

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
        for (int channel = 0; channel < channel_count; channel += 1)
        {
            output_pointer[channel_count * frame + channel] = output->out[channel][frame];
        }
    }

    is_processing = false;
}

AudioOut_MiniAudio::AudioOut_MiniAudio(const std::string &backend_name,
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

int AudioOut_MiniAudio::init()
{
    ma_device_config config = ma_device_config_init(ma_device_type_playback);

    ma_device_info *playback_devices;
    ma_uint32 playback_device_count;
    ma_device_info *capture_devices;
    ma_uint32 capture_device_count;
    ma_context context;
    ma_result rv;

    if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS)
    {
        // Error.
        printf("Error initialising context\n");
        return -1;
    }

    rv = ma_context_get_devices(&context,
                                &playback_devices,
                                &playback_device_count,
                                &capture_devices,
                                &capture_device_count);
    int selected_device_index = -1;
    if (!this->device_name.empty())
    {
        for (int i = 0; i < playback_device_count; i++)
        {
            printf(" - (%d) %s\n", i, playback_devices[i].name);
            if (strcmp(playback_devices[i].name, device_name.c_str()) == 0)
            {
                selected_device_index = i;
            }
        }
        if (selected_device_index == -1)
        {
            printf("Couldn't find device\n");
            return -1;
        }

        config.playback.pDeviceID = &playback_devices[selected_device_index].id;
    }

    // Set to ma_format_unknown to use the device's native format.
    config.playback.format = ma_format_f32;

    // Set to 0 to use the device's native channel count / buffer size.
    config.playback.channels = 0;
    config.periodSizeInFrames = buffer_size;

    // Note that the underlying connection always uses the device's native sample rate.
    // Setting values other than zero instantiates miniaudio's internal resampler.
    config.sampleRate = sample_rate;
    config.dataCallback = data_callback;

    // Buffer blocks into a fixed number of frames
    config.noFixedSizedCallback = 1;

    rv = ma_device_init(NULL, &config, &device);
    if (rv != MA_SUCCESS)
    {
        printf("Error initialising device\n");
        return -1;
    }

    this->sample_rate = device.playback.internalSampleRate;
    this->set_channels(device.playback.internalChannels, 0);

    std::string s = device.playback.internalChannels == 1 ? "" : "s";

    std::cerr << "[MINIAUDIO] Output device: " << std::string(device.playback.name) << " (" << device.playback.internalSampleRate << "Hz, "
              << "buffer size " << device.playback.internalPeriodSizeInFrames << " samples, " << device.playback.internalChannels << " channel" << s << ")"
              << std::endl;

    // do we need to set num_output channels to allocate the right number of output buffers?
    this->set_channels(device.playback.internalChannels, 0);

    return 0;
}

int AudioOut_MiniAudio::start()
{
    ma_result rv = ma_device_start(&device);
    if (rv != MA_SUCCESS)
    {
        printf("Error starting device\n");
        return -1;
    }
    this->set_state(SIGNALFLOW_NODE_STATE_ACTIVE);
    return 0;
}

int AudioOut_MiniAudio::stop()
{
    // TODO
    this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
    return 0;
}

int AudioOut_MiniAudio::destroy()
{
    while (is_processing)
    {
    }

    ma_device_uninit(&device);

    return 0;
}

std::list<std::string> AudioOut_MiniAudio::get_output_device_names()
{
    return {};
}

int AudioOut_MiniAudio::get_default_output_device_index()
{
    return 0;
}

std::list<std::string> AudioOut_MiniAudio::get_output_backend_names()
{
    return {};
}

} // namespace signalflow

#endif
