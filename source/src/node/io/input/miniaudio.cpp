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
    /*--------------------------------------------------------------------------------
     * Write every device channel into the shared ring, then publish the new frames.
     * Lock-free: AudioIn nodes read from the ring on the graph's audio thread.
     *-------------------------------------------------------------------------------*/
    uint64_t written = this->frames_written.load(std::memory_order_relaxed);
    for (ma_uint32 frame = 0; frame < frame_count; frame++)
    {
        size_t index = (written + frame) % this->ring_capacity;
        memcpy(&this->ring[index * this->ring_channels], &input_samples[frame * this->ring_channels], sizeof(sample) * this->ring_channels);
    }
    this->frames_written.store(written + frame_count, std::memory_order_release);
}

void AudioInputManager::begin_block(AudioGraph *graph, int num_frames)
{
    this->current_block = graph->get_render_count();

    uint64_t written = this->frames_written.load(std::memory_order_acquire);
    unsigned int block = std::max(this->period_size, (unsigned int) num_frames);

    /*--------------------------------------------------------------------------------
     * Read one block plus one device period behind the writer, so that a little
     * jitter between the input and output callbacks doesn't cause underruns.
     * If more than a few blocks further behind than that, resync rather than
     * letting latency build up.
     *-------------------------------------------------------------------------------*/
    uint64_t target_lag = (uint64_t) num_frames + this->period_size;
    uint64_t max_lag = target_lag + 4 * (uint64_t) block;

    if (this->reader_synced)
    {
        this->read_frame += this->last_block_frames;
        bool underrun = this->read_frame + num_frames > written;
        bool overrun = !underrun && written - this->read_frame > max_lag;
        if (underrun || overrun)
        {
            this->reader_synced = false;
        }
    }

    if (!this->reader_synced && written >= target_lag)
    {
        this->read_frame = written - target_lag;
        this->reader_synced = true;
    }

    this->last_block_frames = num_frames;

    /*--------------------------------------------------------------------------------
     * The writer may be up to one callback ahead of `written`, so the frames being
     * read must stay clear of that region of the ring.
     *-------------------------------------------------------------------------------*/
    this->block_valid = this->reader_synced && (max_lag + block) <= this->ring_capacity;
}

void AudioInputManager::read(AudioIn *input, Buffer &out, int num_frames)
{
    AudioGraph *graph = input->get_graph();
    if (graph->get_render_count() != this->current_block)
    {
        this->begin_block(graph, num_frames);
    }

    if (!this->block_valid)
    {
        for (unsigned int channel = 0; channel < input->num_channels; channel++)
        {
            memset(out[channel], 0, num_frames * sizeof(sample));
        }
        return;
    }

    for (int frame = 0; frame < num_frames; frame++)
    {
        size_t index = (this->read_frame + frame) % this->ring_capacity;
        const sample *frame_samples = &this->ring[index * this->ring_channels + input->first_channel];
        for (unsigned int channel = 0; channel < input->num_channels; channel++)
        {
            out[channel][frame] = frame_samples[channel];
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

    std::lock_guard<std::mutex> inputs_lock(this->inputs_mutex);
    this->inputs.push_back(input);
}

void AudioInputManager::remove_input(AudioIn *input)
{
    std::lock_guard<std::mutex> lock(this->device_mutex);

    {
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
    char actual_device_name[256];      
    size_t name_len;            
    rv = ma_device_get_name(&device, ma_device_type_capture, actual_device_name, sizeof(actual_device_name) / sizeof(char), &name_len);
    unsigned int num_channels = this->device.capture.channels;

    std::string s = num_channels == 1 ? "" : "s";
    std::cerr << "[miniaudio] Input device: " << std::string(actual_device_name) << " (" << device.capture.internalSampleRate << "Hz, "
              << "buffer size " << device.capture.internalPeriodSizeInFrames << " samples, " << num_channels << " channel" << s << ")"
              << std::endl;

    /*--------------------------------------------------------------------------------
     * Allocate the shared input ring (see miniaudio.h) before the device starts.
     * Leave room for several blocks of lag plus a callback in flight, at whichever
     * is larger of the device period and the graph's block size.
     *-------------------------------------------------------------------------------*/
    this->period_size = std::max(this->device.capture.internalPeriodSizeInFrames, (ma_uint32) 1);
    unsigned int block = std::max(this->period_size, (unsigned int) graph->get_output_buffer_size());
    unsigned int capacity = std::max(block * 16, (unsigned int) 8192);
    this->ring.assign((size_t) capacity * num_channels, 0.0f);
    this->ring_capacity = capacity;
    this->ring_channels = num_channels;

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

    AudioInputManager::get_shared_manager()->read(this, out, num_samples);
}

}
