#pragma once

#include <atomic>
#include <cstdint>
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
 *
 * Input samples are written by the device callback into a single ring buffer
 * holding all device channels. Every AudioIn reads from it at one read position
 * per graph block (identified by AudioGraph::get_render_count()), so all AudioIn
 * nodes are sample-aligned, whenever they were created and however long they went
 * unrendered: previously each AudioIn had its own queue, which filled from the
 * moment it was created but only drained once it was rendered, leaving it
 * permanently delayed relative to AudioIn nodes created earlier.
 *
 * The read position trails the write position by one graph block plus one device
 * period (to buffer against jitter between the input and output callbacks). If the
 * reader falls too far behind or overtakes the writer (e.g. while the input device
 * is closed, or through clock drift between devices), it resynchronises.
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

    /*--------------------------------------------------------------------------------
     * Called by AudioIn::process() on the audio thread: writes num_frames of the
     * input's channels, for the graph block currently being rendered, to out.
     * Outputs silence if no input is available yet.
     *-------------------------------------------------------------------------------*/
    void read(AudioIn *input, Buffer &out, int num_frames);

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

    /*--------------------------------------------------------------------------------
     * Interleaved ring of ring_capacity frames x ring_channels, sized when the device
     * is opened (before any AudioIn is started, so no reader is active).
     * frames_written counts every frame written, across device reopens: the callback
     * writes the frames, then publishes them by advancing it.
     *-------------------------------------------------------------------------------*/
    std::vector<sample> ring;
    unsigned int ring_capacity = 0;
    unsigned int ring_channels = 0;
    unsigned int period_size = 0;
    std::atomic<uint64_t> frames_written { 0 };

    /*--------------------------------------------------------------------------------
     * Reader state, only accessed on the graph's audio thread.
     *-------------------------------------------------------------------------------*/
    uint64_t read_frame = 0;
    uint64_t current_block = UINT64_MAX;
    bool reader_synced = false;
    bool block_valid = false;
    int last_block_frames = 0;

    void begin_block(AudioGraph *graph, int num_frames);
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

    unsigned int num_channels;
    unsigned int first_channel;
    std::atomic<bool> is_started { false };
};

}
