#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/exceptions.h"

#include <stdlib.h>
#include <string>

using namespace signalflow;

typedef float sample;
typedef sample *frame;

typedef RingBuffer<sample> SampleRingBuffer;

#if defined(__APPLE__) && !defined(FFT_FFTW)
#define FFT_ACCELERATE
#else
#define FFT_FFTW
#endif

/*------------------------------------------------------------------------
 * Max supported number of output channels. Impacts memory usage.
 * TODO: Retire this constant.
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_MAX_CHANNELS 32

/*------------------------------------------------------------------------
 * Initial number of output buffers to allocate per node
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_NODE_INITIAL_OUTPUT_BUFFERS 32

/*------------------------------------------------------------------------
 * Max supported number of FFT bins.
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_DEFAULT_FFT_SIZE 1024
#define SIGNALFLOW_MAX_FFT_SIZE 131072
#define SIGNALFLOW_DEFAULT_FFT_HOP_SIZE 128
#define SIGNALFLOW_DEFAULT_FFT_WINDOW_SIZE 0
#define SIGNALFLOW_DEFAULT_FFT_DO_WINDOW true

/*------------------------------------------------------------------------
 * Default sample block size unless otherwise specified.
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_DEFAULT_SAMPLE_RATE 44100

/*------------------------------------------------------------------------
 * Default sample block size unless otherwise specified.
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_DEFAULT_BLOCK_SIZE 256

/*------------------------------------------------------------------------
 * The default size of a node's output buffer, in samples.
 * Needed because nodes such as AudioOut may be instantiated before the
 * audio hardware's sample rate is known.
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_NODE_BUFFER_SIZE 2048

/*------------------------------------------------------------------------
 * The default trigger name, used when node->trigger() is called
 * without any parameters.
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_DEFAULT_TRIGGER "trigger"
#define SIGNALFLOW_RESET_TRIGGER "reset"

/*------------------------------------------------------------------------
 * Macros to aid in processing trigger inputs, for sample-rate and
 * block-rate triggers.
 *
 * SIGNALFLOW_CHECK_TRIGGER checks whether the specified frame of a given
 *     input is a positive zero-crossing, returning true or false
 * SIGNALFLOW_CHECK_CHANNEL_TRIGGER ditto, for a specific channel
 * SIGNALFLOW_PROCESS_TRIGGER checks whether the specified frame of a given
 *     input is a positive zero-crossing, and performs this->trigger(name) if so
 * SIGNALFLOW_PROCESS_TRIGGER_BLOCK repeats the above operation over
 *     num_frames frames of the given input
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_CHECK_TRIGGER(input, frame) \
    SIGNALFLOW_CHECK_CHANNEL_TRIGGER(input, 0, frame)

#define SIGNALFLOW_CHECK_CHANNEL_TRIGGER(input, channel, frame) \
    (input && input->out[channel][frame] > 0 && (frame > 0 ? input->out[channel][frame - 1] <= 0 : input->last_sample[channel] <= 0))

#define SIGNALFLOW_PROCESS_TRIGGER(input, frame, name)   \
    if (input && SIGNALFLOW_CHECK_TRIGGER(input, frame)) \
    {                                                    \
        this->trigger(name);                             \
    }
#define SIGNALFLOW_PROCESS_TRIGGER_BLOCK(input, num_frames, name) \
    if (input)                                                    \
    {                                                             \
        for (int frame = 0; frame < num_frames; frame++)          \
        {                                                         \
            SIGNALFLOW_PROCESS_TRIGGER(input, frame, name);       \
        }                                                         \
    }

#define SIGNALFLOW_CHECK_GRAPH()                         \
    if (!this->graph)                                    \
    {                                                    \
        throw signalflow::graph_not_created_exception(); \
    }

#define SIGNALFLOW_CHECK_NUM_FRAMES()                                                                   \
    if (num_frames > this->output_buffer_length && out == this->out)                                    \
    {                                                                                                   \
        throw std::runtime_error("Error in call to Node::process: Attempted to write too many frames"); \
    }

#define SIGNALFLOW_USER_DIR std::string(getenv("HOME")) + "/.signalflow"

/**------------------------------------------------------------------------
 * Algorithm to use when interpolating between samples.
 *
 * Specifying explicit underlying type allows us to forward-declare
 * enums in class files.
 *------------------------------------------------------------------------*/
enum signalflow_interpolation_mode_t : unsigned int
{
    SIGNALFLOW_INTERPOLATION_NONE,
    SIGNALFLOW_INTERPOLATION_LINEAR,
    SIGNALFLOW_INTERPOLATION_COSINE
};

enum signalflow_event_distribution_t : unsigned int
{
    SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM,
    SIGNALFLOW_EVENT_DISTRIBUTION_POISSON
};

typedef enum : unsigned int
{
    SIGNALFLOW_SCALE_LIN_LIN,
    SIGNALFLOW_SCALE_LIN_EXP,
    SIGNALFLOW_SCALE_EXP_LIN,
    SIGNALFLOW_SCALE_EXP_EXP
} signalflow_scale_t;

typedef enum : unsigned int
{
    SIGNALFLOW_CURVE_LINEAR,
    SIGNALFLOW_CURVE_EXPONENTIAL
} signalflow_curve_t;

typedef enum
{
    SIGNALFLOW_FILTER_TYPE_LOW_PASS,
    SIGNALFLOW_FILTER_TYPE_HIGH_PASS,
    SIGNALFLOW_FILTER_TYPE_BAND_PASS,
    SIGNALFLOW_FILTER_TYPE_NOTCH,
    SIGNALFLOW_FILTER_TYPE_PEAK,
    SIGNALFLOW_FILTER_TYPE_LOW_SHELF,
    SIGNALFLOW_FILTER_TYPE_HIGH_SHELF
} signalflow_filter_type_t;

#define N_CHANNELS -1
