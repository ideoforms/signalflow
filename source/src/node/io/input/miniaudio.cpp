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
std::vector<SampleRingQueue *> input_queue;

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
            input_queue[channel]->append(input_samples[frame * num_channels + channel]);
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
    ma_result rv;
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format = ma_format_f32;
    config.capture.channels = this->num_channels;
    config.periodSizeInFrames = this->get_graph()->get_output_buffer_size();
    config.sampleRate = this->get_graph()->get_sample_rate();
    config.dataCallback = read_callback;

    ma_device_info *capture_devices;
    ma_uint32 capture_device_count;

    // TODO: Add get_input_backend_name
    AudioOut::init_context(&this->context, this->get_graph()->get_config().get_output_backend_name());

    rv = ma_context_get_devices(&this->context,
                                NULL,
                                NULL,
                                &capture_devices,
                                &capture_device_count);
    int selected_device_index = -1;
    std::string device_name = this->get_graph()->get_config().get_input_device_name();

    if (!device_name.empty())
    {
        for (unsigned int i = 0; i < capture_device_count; i++)
        {
            /*-----------------------------------------------------------------------*
             * For ease of use, SignalFlow allows for partial matches so that only
             * the first part of the device names needs to be specified. However,
             * an errors is thrown if the match is ambiguous.
             *-----------------------------------------------------------------------*/
            if (strncmp(capture_devices[i].name, device_name.c_str(), strlen(device_name.c_str())) == 0)
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

        config.capture.pDeviceID = &capture_devices[selected_device_index].id;
    }

    rv = ma_device_init(NULL, &config, &device);
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

    for (int channel = 0; channel < device.capture.internalChannels; channel++)
    {
        SampleRingQueue *queue = new SampleRingQueue(device.capture.internalPeriodSizeInFrames * 8);
        /*--------------------------------------------------------------------------------
         * Initialise the queue with single block of silence, ensuring that the write
         * head is always ahead of the read head by a block. This adds a single block
         * of latency between input and output, but buffers against jitter in the
         * case that two reads occur between one write (as experienced on Linux/alsa).
         *-------------------------------------------------------------------------------*/
        std::vector<float> silence(device.capture.internalPeriodSizeInFrames, 0);
        queue->extend(silence);
        input_queue.push_back(queue);
    }

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
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_samples; frame++)
        {
            out[channel][frame] = input_queue[channel]->pop();
        }
    }
}

}
