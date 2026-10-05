#include "signalflow/node/io/input/miniaudio.h"

#include "signalflow/core/graph.h"
#include "signalflow/node/io/output/miniaudio.h"

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string.h>

namespace signalflow
{

/*--------------------------------------------------------------------------------
 * AudioInputManager
 *-------------------------------------------------------------------------------*/

AudioInputManager *AudioInputManager::get_shared_manager()
{
    /*--------------------------------------------------------------------------------
     * Intentionally never deleted, so that it outlives any AudioIn nodes that are
     * destroyed during static destruction.
     *-------------------------------------------------------------------------------*/
    static AudioInputManager *shared_manager = new AudioInputManager();
    return shared_manager;
}

void AudioInputManager::read_callback(ma_device *pDevice,
                                      void *pOutput,
                                      const void *pInput,
                                      ma_uint32 frameCount)
{
    AudioInputManager *manager = (AudioInputManager *) pDevice->pUserData;
    manager->process_input((const float *) pInput, frameCount);
}

void AudioInputManager::process_input(const float *input_samples, ma_uint32 frame_count)
{
    std::lock_guard<std::mutex> lock(this->inputs_mutex);

    unsigned int device_channels = this->device.capture.channels;

    for (AudioIn *input : this->inputs)
    {
        for (unsigned int frame = 0; frame < frame_count; frame++)
        {
            for (unsigned int channel = 0; channel < input->num_channels; channel++)
            {
                unsigned int device_channel = input->first_channel + channel;
                input->queues[channel]->append(input_samples[frame * device_channels + device_channel]);
            }
        }
    }
}

void AudioInputManager::add_input(AudioIn *input)
{
    std::lock_guard<std::mutex> lock(this->device_mutex);

    if (!this->is_device_open)
    {
        this->open_device(input->get_graph());
    }

    unsigned int device_channels = this->device.capture.channels;
    if (input->first_channel + input->num_channels > device_channels)
    {
        if (this->inputs.empty())
        {
            this->close_device();
        }
        throw audio_io_exception("AudioIn: Requested channels " + std::to_string(input->first_channel) + "-" + std::to_string(input->first_channel + input->num_channels - 1) + ", but input device only has " + std::to_string(device_channels) + " channel" + (device_channels == 1 ? "" : "s"));
    }

    /*--------------------------------------------------------------------------------
     * Initialise the queue with single block of silence, ensuring that the write
     * head is always ahead of the read head by a block. This adds a single block
     * of latency between input and output, but buffers against jitter in the
     * case that two reads occur between one write (as experienced on Linux/alsa).
     *-------------------------------------------------------------------------------*/
    unsigned int period_size = this->device.capture.internalPeriodSizeInFrames;
    input->init_queues(period_size * 8, period_size);

    std::lock_guard<std::mutex> inputs_lock(this->inputs_mutex);
    this->inputs.push_back(input);
}

void AudioInputManager::remove_input(AudioIn *input)
{
    std::lock_guard<std::mutex> lock(this->device_mutex);

    {
        /*--------------------------------------------------------------------------------
         * Once the input has been removed under the inputs lock, the audio callback
         * is guaranteed to no longer be accessing its queues.
         *-------------------------------------------------------------------------------*/
        std::lock_guard<std::mutex> inputs_lock(this->inputs_mutex);
        this->inputs.erase(std::remove(this->inputs.begin(), this->inputs.end(), input), this->inputs.end());
    }

    if (this->inputs.empty() && this->is_device_open)
    {
        this->close_device();
    }
}

unsigned int AudioInputManager::get_num_channels()
{
    std::lock_guard<std::mutex> lock(this->device_mutex);
    return this->is_device_open ? this->device.capture.channels : 0;
}

void AudioInputManager::open_device(AudioGraph *graph)
{
    ma_result rv;
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format = ma_format_f32;

    /*--------------------------------------------------------------------------------
     * Open the device at its native channel count. If a channel count is requested
     * that differs from the device's, miniaudio performs channel conversion, which
     * (for mono output) averages all input channels, attenuating the signal.
     *-------------------------------------------------------------------------------*/
    config.capture.channels = 0;
    config.periodSizeInFrames = graph->get_output_buffer_size();
    config.sampleRate = graph->get_sample_rate();
    config.dataCallback = AudioInputManager::read_callback;
    config.pUserData = this;

    ma_device_info *capture_devices;
    ma_uint32 capture_device_count;

    // TODO: Add get_input_backend_name
    AudioOut::init_context(&this->context, graph->get_config().get_backend_name());

    rv = ma_context_get_devices(&this->context,
                                NULL,
                                NULL,
                                &capture_devices,
                                &capture_device_count);
    if (rv != MA_SUCCESS)
    {
        ma_context_uninit(&this->context);
        throw audio_io_exception("miniaudio: Failure querying audio devices");
    }

    int selected_device_index = -1;
    std::string device_name = graph->get_config().get_input_device_name();

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
                    ma_context_uninit(&this->context);
                    throw audio_io_exception("More than one audio device found matching name '" + device_name + "'");
                }
                selected_device_index = i;
            }
        }
        if (selected_device_index == -1)
        {
            ma_context_uninit(&this->context);
            throw audio_io_exception("No audio device found matching name '" + device_name + "'");
        }

        config.capture.pDeviceID = &capture_devices[selected_device_index].id;
    }

    rv = ma_device_init(&this->context, &config, &this->device);
    if (rv != MA_SUCCESS)
    {
        ma_context_uninit(&this->context);
        throw audio_io_exception("miniaudio: Error initialising input device");
    }

    /*--------------------------------------------------------------------------------
     * Note that the underlying sample rate used by the recording hardware
     * (`device.capture.internalSampleRate`) may not be the same as that used
     * by `AudioIn`: SignalFlow requires that the input and output streams are both
     * on the same sample rate, so miniaudio's resampling is used to unify them.
     *-------------------------------------------------------------------------------*/
    unsigned int num_channels = this->device.capture.channels;
    std::string s = num_channels == 1 ? "" : "s";
    std::cerr << "[miniaudio] Input device: " << std::string(this->device.capture.name) << " (" << this->device.capture.internalSampleRate << "Hz, "
              << "buffer size " << this->device.capture.internalPeriodSizeInFrames << " samples, " << num_channels << " channel" << s << ")"
              << std::endl;

    rv = ma_device_start(&this->device);
    if (rv != MA_SUCCESS)
    {
        ma_device_uninit(&this->device);
        ma_context_uninit(&this->context);
        throw audio_io_exception("miniaudio: Error starting input device");
    }

    this->is_device_open = true;
}

void AudioInputManager::close_device()
{
    // ma_device_uninit stops the device and waits for any in-progress callback.
    ma_device_uninit(&this->device);
    ma_context_uninit(&this->context);
    this->is_device_open = false;
}

/*--------------------------------------------------------------------------------
 * AudioIn
 *-------------------------------------------------------------------------------*/

AudioIn::AudioIn(unsigned int num_channels, unsigned int first_channel)
    : AudioIn_Abstract()
{
    this->name = "audioin-miniaudio";
    this->num_channels = num_channels;
    this->first_channel = first_channel;
    this->init();
}

AudioIn::~AudioIn()
{
    this->destroy();
    this->free_queues();
}

void AudioIn::init()
{
    if (this->num_channels == 0)
    {
        throw audio_io_exception("AudioIn: num_channels must be at least 1");
    }

    this->set_channels(0, this->num_channels);
    this->start();
}

void AudioIn::start()
{
    if (!this->is_started)
    {
        AudioInputManager::get_shared_manager()->add_input(this);
        this->is_started = true;
    }
}

void AudioIn::stop()
{
    if (this->is_started)
    {
        /*--------------------------------------------------------------------------------
         * Queues are not freed here, as process() may still be reading from them
         * on the audio thread. They are freed in the destructor.
         *-------------------------------------------------------------------------------*/
        this->is_started = false;
        AudioInputManager::get_shared_manager()->remove_input(this);
    }
}

void AudioIn::destroy()
{
    this->stop();
}

unsigned int AudioIn::get_first_channel() const
{
    return this->first_channel;
}

void AudioIn::init_queues(unsigned int queue_size, unsigned int latency)
{
    /*--------------------------------------------------------------------------------
     * If restarting after a stop(), reuse the existing queues, which may still be
     * read by process() on the audio thread.
     *-------------------------------------------------------------------------------*/
    if (!this->queues.empty())
    {
        return;
    }

    for (unsigned int channel = 0; channel < this->num_channels; channel++)
    {
        SampleRingQueue *queue = new SampleRingQueue(queue_size);
        std::vector<float> silence(latency, 0);
        queue->extend(silence);
        this->queues.push_back(queue);
    }
}

void AudioIn::free_queues()
{
    for (SampleRingQueue *queue : this->queues)
    {
        delete queue;
    }
    this->queues.clear();
}

void AudioIn::process(Buffer &out, int num_samples)
{
    if (!this->is_started)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            memset(out[channel], 0, num_samples * sizeof(sample));
        }
        return;
    }

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_samples; frame++)
        {
            out[channel][frame] = this->queues[channel]->pop();
        }
    }
}

}
