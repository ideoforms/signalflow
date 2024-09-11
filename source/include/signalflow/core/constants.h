#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/exceptions.h"

#include <limits>
#include <map>
#include <stdlib.h>
#include <string>

using namespace signalflow;

typedef float sample;
typedef sample *frame;

typedef RingBuffer<sample> SampleRingBuffer;

#if defined(__APPLE__) && !defined(FFT_FFTW)
#define FFT_ACCELERATE
#endif

/*------------------------------------------------------------------------
 * Absent macro definitions in Windows
 *-----------------------------------------------------------------------*/
#if defined(WIN32) && !defined(M_PI)
#define M_PI 3.14159265358979323846
#endif

#define UNUSED(expr)   \
    do                 \
    {                  \
        (void) (expr); \
    } while (0)

/*------------------------------------------------------------------------
 * Sentinel value, used to represent an undefined or unset float value.
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_NULL_FLOAT std::numeric_limits<float>::max()

/*------------------------------------------------------------------------
 * Max supported number of output channels. Impacts memory usage.
 * TODO: Retire this constant.
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_MAX_CHANNELS 64

/*------------------------------------------------------------------------
 * Initial number of output channels to allocate per node.
 * More channels are allocated as needed at runtime.
 *-----------------------------------------------------------------------*/
#define SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS 2

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
 * TODO: Review whether this is still needed?
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

#define SIGNALFLOW_TRIGGER_RESET "reset"
#define SIGNALFLOW_TRIGGER_PLAY "play"
#define SIGNALFLOW_TRIGGER_STOP "stop"
#define SIGNALFLOW_TRIGGER_RECORD "record"
#define SIGNALFLOW_TRIGGER_SET_POSITION "set_position"
#define SIGNALFLOW_TRIGGER_SET_LOOP_START "loop_start"
#define SIGNALFLOW_TRIGGER_SET_LOOP_END "loop_end"
#define SIGNALFLOW_TRIGGER_EXPLORE "explore"
#define SIGNALFLOW_TRIGGER_GENERATE "generate"
#define SIGNALFLOW_TRIGGER_PLAY_SEGMENT "play_segment"

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

#ifdef _WIN32
#define SIGNALFLOW_USER_DIR (std::string(getenv("HOMEPATH")) + "/.signalflow")
#else
#define SIGNALFLOW_USER_DIR (std::string(getenv("HOME")) + "/.signalflow")
#endif

/**------------------------------------------------------------------------
 * Algorithm to use when interpolating between samples.
 *
 * Specifying explicit underlying type allows us to forward-declare
 * enums in class files.
 *------------------------------------------------------------------------*/
enum signalflow_interpolation_mode_t : unsigned int
{
    SIGNALFLOW_INTERPOLATION_MODE_NONE,
    SIGNALFLOW_INTERPOLATION_MODE_LINEAR,
    SIGNALFLOW_INTERPOLATION_MODE_COSINE
};

enum signalflow_event_distribution_t : unsigned int
{
    SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM,
    SIGNALFLOW_EVENT_DISTRIBUTION_POISSON
};

static std::map<std::string, signalflow_event_distribution_t> SIGNALFLOW_EVENT_DISTRIBUTION_MAP {
    { "uniform", SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM },
    { "poisson", SIGNALFLOW_EVENT_DISTRIBUTION_POISSON }
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

typedef enum : unsigned int
{
    SIGNALFLOW_SEGMENT_SHAPE_LINEAR,
    SIGNALFLOW_SEGMENT_SHAPE_EXPONENTIAL,
    SIGNALFLOW_SEGMENT_SHAPE_SINE,
} signalflow_segment_shape_t;

typedef enum : unsigned int
{
    SIGNALFLOW_FILTER_TYPE_LOW_PASS,
    SIGNALFLOW_FILTER_TYPE_HIGH_PASS,
    SIGNALFLOW_FILTER_TYPE_BAND_PASS,
    SIGNALFLOW_FILTER_TYPE_NOTCH,
    SIGNALFLOW_FILTER_TYPE_PEAK,
    SIGNALFLOW_FILTER_TYPE_LOW_SHELF,
    SIGNALFLOW_FILTER_TYPE_HIGH_SHELF
} signalflow_filter_type_t;

static std::map<std::string, signalflow_filter_type_t> SIGNALFLOW_FILTER_TYPE_MAP {
    { "low_pass", SIGNALFLOW_FILTER_TYPE_LOW_PASS },
    { "high_pass", SIGNALFLOW_FILTER_TYPE_HIGH_PASS },
    { "band_pass", SIGNALFLOW_FILTER_TYPE_BAND_PASS },
    { "notch", SIGNALFLOW_FILTER_TYPE_NOTCH },
    { "peak", SIGNALFLOW_FILTER_TYPE_PEAK },
    { "low_shelf", SIGNALFLOW_FILTER_TYPE_LOW_SHELF },
    { "high_shelf", SIGNALFLOW_FILTER_TYPE_HIGH_SHELF }
};

#define N_CHANNELS -1
