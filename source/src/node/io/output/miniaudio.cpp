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

std::unordered_map<std::string, ma_device_backend_vtable *> possible_backend_names = {
    { "wasapi", ma_device_backend_wasapi },
    { "dsound", ma_device_backend_dsound },
    { "winmm", ma_device_backend_winmm },
    { "coreaudio", ma_device_backend_coreaudio },
    { "sndio", ma_device_backend_sndio },
    { "audio4", ma_device_backend_audio4 },
    { "oss", ma_device_backend_oss },
    { "pulseaudio", ma_device_backend_pulseaudio },
    { "alsa", ma_device_backend_alsa },
    { "jack", ma_device_backend_jack },
    { "aaudio", ma_device_backend_aaudio },
    { "opensl", ma_device_backend_opensl },
    { "webaudio", ma_device_backend_webaudio },
    { "null", ma_device_backend_null },
};

void data_callback(ma_device *ma_device_ptr,
                   void *ma_output_pointer,
                   const void *ma_input_pointer,
                   ma_uint32 ma_frame_count)
{
    is_processing = true;
    int channel_count = ma_device_ptr->playback.channels;

    /*-----------------------------------------------------------------------*
     * Do nothing if the shared_graph hasn't been initialized yet.
     *-----------------------------------------------------------------------*/
    if (!shared_graph || !shared_graph->get_output())
    {
        return;
    }

    float *output_pointer = (float *) ma_output_pointer;

    try
    {
        shared_graph->render(ma_frame_count);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in AudioGraph: " << e.what() << std::endl;
        exit(1);
    }

    NodeRef output = shared_graph->get_output();
    for (unsigned int frame = 0; frame < ma_frame_count; frame++)
    {
        for (int channel = 0; channel < channel_count; channel += 1)
        {
            output_pointer[channel_count * frame + channel] = output->out[channel][frame];
        }
    }

    is_processing = false;
}

AudioOut::AudioOut(const std::string &backend_name,
                   const std::string &device_name,
                   unsigned int sample_rate,
                   unsigned int buffer_size,
                   unsigned int num_channels)
    : AudioOut_Abstract()
{
    this->backend_name = backend_name;
    this->device_name = device_name;
    this->sample_rate = sample_rate;
    this->buffer_size = buffer_size;
    this->num_channels = num_channels;
    this->name = "audioout";

    this->init();
}

void AudioOut::init()
{
    ma_device_config config = ma_device_config_init(ma_device_type_playback);

    ma_device_info *playback_devices;
    ma_uint32 playback_device_count;
    ma_result rv;

    AudioOut::init_context(&this->context, this->backend_name);

    rv = ma_context_get_devices(&this->context,
                                &playback_devices,
                                &playback_device_count,
                                NULL,
                                NULL);
    int selected_device_index = -1;
    if (!this->device_name.empty())
    {
        for (unsigned int i = 0; i < playback_device_count; i++)
        {
            /*-----------------------------------------------------------------------*
             * For ease of use, SignalFlow allows for partial matches so that only
             * the first part of the device names needs to be specified. However,
             * an errors is thrown if the match is ambiguous.
             *-----------------------------------------------------------------------*/
            if (strncmp(playback_devices[i].name, device_name.c_str(), strlen(device_name.c_str())) == 0)
            {
                if (selected_device_index != -1)
                {
                    throw audio_io_exception("More than one audio device found matching name '" + device_name + "'");
                }
                selected_device_index = i;
            }
        }
        if (selected_device_index == -1)
        {
            throw audio_io_exception("No audio device found matching name '" + device_name + "'");
        }

        config.playback.pDeviceID = &playback_devices[selected_device_index].id;
    }

    // Set to ma_format_unknown to use the device's native format.
    config.playback.format = ma_format_f32;

    // Set to 0 to use the device's native channel count.
    config.playback.channels = num_channels;

    // Set to 0 to use the device's native buffer size.
    config.periodSizeInFrames = buffer_size;

    // Note that the underlying connection always uses the device's native sample rate.
    // Setting values other than zero instantiates miniaudio's internal resampler.
    config.sampleRate = this->sample_rate;
    config.dataCallback = data_callback;

    // On Core Audio, let the application select a preferred sample rate.
    config.coreaudio.allowNominalSampleRateChange = 1;

    rv = ma_device_init(&this->context, &config, &device);
    if (rv != MA_SUCCESS)
    {
        throw audio_io_exception("miniaudio: Error initialising output device");
    }

    this->set_channels(device.playback.internalChannels, 0);

    /*--------------------------------------------------------------------------------
     * If no specified sample rate was given, update AudioOut's sample rate to
     * reflect the actual underlying sample rate.
     *
     * Otherwise, SignalFlow will use the user-specified sample rate, and miniaudio
     * will perform sample-rate conversion.
     *-------------------------------------------------------------------------------*/
    if (this->sample_rate == 0)
    {
        this->sample_rate = device.playback.internalSampleRate;
    }

    /*--------------------------------------------------------------------------------
     * Update AudioOut's buffer size to reflect the actual underlying buffer size.
     *-------------------------------------------------------------------------------*/
    this->buffer_size = device.playback.internalPeriodSizeInFrames;

    char device_name[256];
    size_t name_len;
    rv = ma_device_get_name(&device, ma_device_type_playback, device_name, sizeof(device_name) / sizeof(char), &name_len);

    std::string s = device.playback.internalChannels == 1 ? "" : "s";
    std::cerr << "[miniaudio] Output device: " << std::string(device_name) << " (" << device.playback.internalSampleRate << "Hz, "
              << "buffer size " << this->buffer_size << " samples, " << device.playback.internalChannels << " channel" << s << ")"
              << std::endl;
}

void AudioOut::start()
{
    ma_result rv = ma_device_start(&device);
    if (rv != MA_SUCCESS)
    {
        throw audio_io_exception("miniaudio: Error starting output device");
    }
    this->set_state(SIGNALFLOW_NODE_STATE_ACTIVE);
}

void AudioOut::stop()
{
    ma_result rv = ma_device_stop(&device);
    if (rv != MA_SUCCESS)
    {
        throw audio_io_exception("miniaudio: Error stopping output device");
    }
    this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
}

void AudioOut::destroy()
{
    while (is_processing)
    {
    }

    ma_device_uninit(&device);
}

// static
void AudioOut::init_context(ma_context *context, std::string backend_name)
{
    if (!backend_name.empty())
    {
        if (possible_backend_names.find(backend_name) == possible_backend_names.end())
        {
            throw audio_io_exception("miniaudio: Backend name not recognised: " + backend_name);
        }

        ma_device_backend_vtable *backend = possible_backend_names[backend_name];
        if (backend == NULL)
        {
            throw audio_io_exception("miniaudio: Backend not supported: " + backend_name);
        }

        ma_device_backend_config backend_config = ma_device_backend_config_init(backend, NULL);

        if (ma_context_init(&backend_config, 1, NULL, context) != MA_SUCCESS)
        {
            throw audio_io_exception("miniaudio: Error initialising context");
        }
    }
    else
    {
        if (ma_context_init(NULL, 0, NULL, context) != MA_SUCCESS)
        {
            throw audio_io_exception("miniaudio: Error initialising context");
        }
    }
}

std::list<std::string> AudioOut::get_output_device_names(std::string backend_name)
{
    std::list<std::string> device_names;

    ma_result rv;
    ma_device_info *playback_devices;
    ma_uint32 playback_device_count;
    ma_context context;

    AudioOut::init_context(&context, backend_name);

    rv = ma_context_get_devices(&context,
                                &playback_devices,
                                &playback_device_count,
                                NULL,
                                NULL);
    if (rv != MA_SUCCESS)
    {
        throw audio_io_exception("miniaudio: Failure querying audio devices");
    }
    for (unsigned int i = 0; i < playback_device_count; i++)
    {
        device_names.push_back(std::string(playback_devices[i].name));
    }

    ma_context_uninit(&context);

    return device_names;
}

std::list<std::string> AudioOut::get_input_device_names(std::string backend_name)
{
    std::list<std::string> device_names;

    ma_result rv;
    ma_device_info *capture_devices;
    ma_uint32 capture_device_count;
    ma_context context;

    AudioOut::init_context(&context, backend_name);

    rv = ma_context_get_devices(&context,
                                NULL,
                                NULL,
                                &capture_devices,
                                &capture_device_count);
    if (rv != MA_SUCCESS)
    {
        throw audio_io_exception("miniaudio: Failure querying audio devices");
    }
    for (unsigned int i = 0; i < capture_device_count; i++)
    {
        device_names.push_back(std::string(capture_devices[i].name));
    }

    ma_context_uninit(&context);

    return device_names;
}

std::list<std::string> AudioOut::get_backend_names()
{
    std::list<std::string> backend_names;

    for (auto pair : possible_backend_names)
    {
        printf("examining %s\n", pair.first.c_str());
        if (pair.second != NULL)
        {
            std::string backend_name = pair.first;
            if (backend_name != "null")
            {
                backend_names.push_back(backend_name);
            }
        }
    }

    return backend_names;
}

} // namespace signalflow
