#include "signalflow/node/io/output/miniaudio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "signalflow/node/io/output/miniaudio-library.h"

#include "signalflow/core/graph.h"

#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_map>

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
    this->name = "audioout-miniaudio";

    this->init();
}

void AudioOut_MiniAudio::init_context(ma_context *context)
{
    if (!this->backend_name.empty())
    {
        std::unordered_map<std::string, ma_backend> possible_backend_names = {
            { "wasapi", ma_backend_wasapi },
            { "dsound", ma_backend_dsound },
            { "ma_backend_winmm", ma_backend_winmm },
            { "coreaudio", ma_backend_coreaudio },
            { "sndio", ma_backend_sndio },
            { "audio4", ma_backend_audio4 },
            { "oss", ma_backend_oss },
            { "pulseaudio", ma_backend_pulseaudio },
            { "alsa", ma_backend_alsa },
            { "jack", ma_backend_jack },
            { "aaudio", ma_backend_aaudio },
            { "opensl", ma_backend_opensl },
            { "webaudio", ma_backend_webaudio },
            { "null", ma_backend_null },
        };

        if (possible_backend_names.find(this->backend_name) == possible_backend_names.end())
        {
            throw std::runtime_error("miniaudio: Backend name not recognised: " + this->backend_name);
        }
        ma_backend backend_name = possible_backend_names[this->backend_name];

        if (ma_context_init(&backend_name, 1, NULL, context) != MA_SUCCESS)
        {
            throw std::runtime_error("miniaudio: Error initialising context");
        }
    }
    else
    {
        if (ma_context_init(NULL, 0, NULL, context) != MA_SUCCESS)
        {
            throw std::runtime_error("miniaudio: Error initialising context");
        }
    }
}

int AudioOut_MiniAudio::init()
{
    ma_device_config config = ma_device_config_init(ma_device_type_playback);

    ma_device_info *playback_devices;
    ma_uint32 playback_device_count;
    ma_device_info *capture_devices;
    ma_uint32 capture_device_count;
    ma_result rv;

    this->init_context(&this->context);

    rv = ma_context_get_devices(&this->context,
                                &playback_devices,
                                &playback_device_count,
                                &capture_devices,
                                &capture_device_count);
    int selected_device_index = -1;
    if (!this->device_name.empty())
    {
        for (int i = 0; i < playback_device_count; i++)
        {
            if (strncmp(playback_devices[i].name, device_name.c_str(), strlen(device_name.c_str())) == 0)
            {
                if (selected_device_index != -1)
                {
                    throw std::runtime_error("More than one audio device found matching name '" + device_name + "'");
                }
                selected_device_index = i;
            }
        }
        if (selected_device_index == -1)
        {
            throw std::runtime_error("No audio device found matching name '" + device_name + "'");
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
        throw std::runtime_error("miniaudio: Error initialising output device");
    }

    this->sample_rate = device.playback.internalSampleRate;
    this->set_channels(device.playback.internalChannels, 0);

    std::string s = device.playback.internalChannels == 1 ? "" : "s";
    std::cerr << "[miniaudio] Output device: " << std::string(device.playback.name) << " (" << device.playback.internalSampleRate << "Hz, "
              << "buffer size " << device.playback.internalPeriodSizeInFrames << " samples, " << device.playback.internalChannels << " channel" << s << ")"
              << std::endl;

    return 0;
}

int AudioOut_MiniAudio::start()
{
    ma_result rv = ma_device_start(&device);
    if (rv != MA_SUCCESS)
    {
        throw std::runtime_error("miniaudio: Error starting output device");
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
    std::list<std::string> device_names;

    ma_result rv;
    ma_device_info *playback_devices;
    ma_uint32 playback_device_count;
    ma_device_info *capture_devices;
    ma_uint32 capture_device_count;
    ma_context context;
    this->init_context(&context);

    rv = ma_context_get_devices(&context,
                                &playback_devices,
                                &playback_device_count,
                                &capture_devices,
                                &capture_device_count);
    if (rv != MA_SUCCESS)
    {
        throw std::runtime_error("miniaudio: Failure querying audio devices");
    }
    for (int i = 0; i < playback_device_count; i++)
    {
        device_names.push_back(std::string(playback_devices[i].name));
    }

    return device_names;
}

int AudioOut_MiniAudio::get_default_output_device_index()
{
    // TODO: Is this even used?
    return -1;
}

std::list<std::string> AudioOut_MiniAudio::get_output_backend_names()
{
    std::list<std::string> backend_names;
    ma_backend enabled_backends[MA_BACKEND_COUNT];
    size_t enabled_backend_count;
    ma_result rv;

    rv = ma_get_enabled_backends(enabled_backends, MA_BACKEND_COUNT, &enabled_backend_count);
    if (rv != MA_SUCCESS)
    {
        throw std::runtime_error("miniaudio: Failure querying backend devices");
    }
    for (int i = 0; i < enabled_backend_count; i++)
    {
        std::string backend_name = std::string(ma_get_backend_name(enabled_backends[i]));
        if (backend_name != "Custom" && backend_name != "Null")
        {
            backend_names.push_back(backend_name);
        }
    }

    return backend_names;
}

} // namespace signalflow
