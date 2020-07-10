#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/exceptions.h"

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
 *-----------------------------------------------------------------------*/
#define SIGNAL_MAX_CHANNELS 32

/*------------------------------------------------------------------------
 * Max supported number of FFT bins.
 *-----------------------------------------------------------------------*/
#define SIGNAL_DEFAULT_FFT_SIZE 1024
#define SIGNAL_MAX_FFT_SIZE 131072
#define SIGNAL_DEFAULT_FFT_HOP_SIZE 128
#define SIGNAL_DEFAULT_FFT_WINDOW_SIZE 0
#define SIGNAL_DEFAULT_FFT_DO_WINDOW true

/*------------------------------------------------------------------------
 * Default sample block size unless otherwise specified.
 *-----------------------------------------------------------------------*/
#define SIGNAL_DEFAULT_SAMPLE_RATE 44100

/*------------------------------------------------------------------------
 * Default sample block size unless otherwise specified.
 *-----------------------------------------------------------------------*/
#define SIGNAL_DEFAULT_BLOCK_SIZE 256

/*------------------------------------------------------------------------
 * The default size of a node's output buffer, in samples.
 * Needed because nodes such as AudioOut may be instantiated before the
 * audio hardware's sample rate is known.
 *-----------------------------------------------------------------------*/
#define SIGNAL_NODE_BUFFER_SIZE 2048

/*------------------------------------------------------------------------
 * The default trigger name, used when node->trigger() is called
 * without any parameters.
 *-----------------------------------------------------------------------*/
#define SIGNAL_DEFAULT_TRIGGER "trigger"

/*------------------------------------------------------------------------
 * Macros to aid in processing trigger inputs, for sample-rate and
 * block-rate triggers.
 *
 * SIGNAL_CHECK_TRIGGER checks whether the specified frame of a given
 *     input is a positive zero-crossing, returning true or false
 * SIGNAL_CHECK_CHANNEL_TRIGGER ditto, for a specific channel
 * SIGNAL_PROCESS_TRIGGER checks whether the specified frame of a given
 *     input is a positive zero-crossing, and performs this->trigger(name) if so
 * SIGNAL_PROCESS_TRIGGER_BLOCK repeats the above operation over
 *     num_frames frames of the given input
 *-----------------------------------------------------------------------*/
#define SIGNAL_CHECK_TRIGGER(input, frame) \
    (input && input->out[0][frame] > 0 && input->out[0][frame - 1] <= 0)

#define SIGNAL_CHECK_CHANNEL_TRIGGER(input, channel, frame) \
    (input && input->out[channel][frame] > 0 && input->out[channel][frame - 1] <= 0)

#define SIGNAL_PROCESS_TRIGGER(input, frame, name)   \
    if (input && SIGNAL_CHECK_TRIGGER(input, frame)) \
    {                                                \
        this->trigger(name);                         \
    }
#define SIGNAL_PROCESS_TRIGGER_BLOCK(input, num_frames, name) \
    if (input)                                                \
    {                                                         \
        for (int frame = 0; frame < num_frames; frame++)      \
        {                                                     \
            SIGNAL_PROCESS_TRIGGER(input, frame, name);       \
        }                                                     \
    }

#define SIGNAL_CHECK_GRAPH()                             \
    if (!this->graph)                                    \
    {                                                    \
        throw signalflow::graph_not_created_exception(); \
    }

#define SIGNAL_CHECK_NUM_FRAMES()                                                                       \
    if (num_frames > SIGNAL_NODE_BUFFER_SIZE && out == this->out)                                       \
    {                                                                                                   \
        throw std::runtime_error("Error in call to Node::process: Attempted to write too many frames"); \
    }

/**------------------------------------------------------------------------
 * Algorithm to use when interpolating between samples.
 *
 * Specifying explicit underlying type allows us to forward-declare
 * enums in class files.
 *------------------------------------------------------------------------*/
enum signal_interpolation_mode_t : unsigned int
{
    SIGNAL_INTERPOLATION_NONE,
    SIGNAL_INTERPOLATION_LINEAR
};

enum signal_event_distribution_t : unsigned int
{
    SIGNAL_EVENT_DISTRIBUTION_UNIFORM,
    SIGNAL_EVENT_DISTRIBUTION_POISSON
};

typedef enum : unsigned int
{
    SIGNAL_SCALE_LIN_LIN,
    SIGNAL_SCALE_LIN_EXP,
    SIGNAL_SCALE_EXP_LIN,
    SIGNAL_SCALE_EXP_EXP
} signal_scale_t;

typedef enum : unsigned int
{
    SIGNAL_CURVE_LINEAR,
    SIGNAL_CURVE_EXPONENTIAL
} signal_curve_t;

#define N_CHANNELS -1
