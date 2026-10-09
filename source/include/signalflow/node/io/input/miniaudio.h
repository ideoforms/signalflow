#pragma once

#include <atomic>
#include <mutex>
#include <vector>

#include "abstract.h"

#include "../output/miniaudio-library.h"
#include "signalflow/core/graph.h"

namespace signalflow
{

class AudioIn;

/*--------------------------------------------------------------------------------
 * AudioInputManager handles the single shared input device. The device is opened
 * at its native channel count, and each registered AudioIn receives the
 * contiguous block of channels specified by its num_channels and first_channel.
 *
 * The device is opened when the first AudioIn is added, and closed when the
 * last AudioIn is removed.
 *-------------------------------------------------------------------------------*/
class AudioInputManager
{
public:
    static AudioInputManager *get_shared_manager();

    /*--------------------------------------------------------------------------------
     * Register an AudioIn, opening the input device if needed.
     * Throws audio_io_exception if the AudioIn's channel range exceeds the
     * device's channel count.
     *-------------------------------------------------------------------------------*/
    void add_input(AudioIn *input);
    void remove_input(AudioIn *input);

    unsigned int get_num_channels();

    static void read_callback(ma_device *pDevice,
                              void *pOutput,
                              const void *pInput,
                              ma_uint32 frameCount);

private:
    AudioInputManager() = default;

    void open_device(AudioGraph *graph);
    void close_device();
    void process_input(const float *input_samples, ma_uint32 frame_count);

    ma_context context;
    ma_device device;
    bool is_device_open = false;
    std::vector<AudioIn *> inputs;

    /*--------------------------------------------------------------------------------
     * inputs_mutex guards the list of inputs, and is taken by the audio callback.
     * device_mutex guards opening/closing the device, and must never be held by
     * the callback (as ma_device_uninit waits for the callback to complete).
     *-------------------------------------------------------------------------------*/
    std::mutex inputs_mutex;
    std::mutex device_mutex;
};

class AudioIn : public AudioIn_Abstract
{
public:
    AudioIn(unsigned int num_channels = 1, unsigned int first_channel = 0);
    virtual ~AudioIn() override;
    virtual void init() override;
    virtual void start() override;
    virtual void stop() override;
    virtual void destroy() override;
    virtual void process(Buffer &out, int num_samples) override;

    unsigned int get_first_channel() const;

private:
    friend class AudioInputManager;

    void init_queues(unsigned int queue_size, unsigned int latency);
    void free_queues();

    unsigned int num_channels;
    unsigned int first_channel;
    std::atomic<bool> is_started { false };
    std::vector<SampleRingQueue *> queues;
};

}
