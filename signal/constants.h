#pragma once

#include "ringbuffer.h"

typedef float sample;
typedef sample * frame;

typedef RingBuffer <sample> SampleRingBuffer;

/*------------------------------------------------------------------------
 * Max supported number of output channels. Impacts memory usage.
 *-----------------------------------------------------------------------*/
#define SIGNAL_MAX_CHANNELS 32

/*------------------------------------------------------------------------
 * Max supported number of FFT bins.
 *-----------------------------------------------------------------------*/
#define SIGNAL_DEFAULT_FFT_SIZE 1024
#define SIGNAL_MAX_FFT_SIZE 8192

/*------------------------------------------------------------------------
 * Default sample block size unless otherwise specified.
 *-----------------------------------------------------------------------*/
#define SIGNAL_DEFAULT_BLOCK_SIZE 256

/*------------------------------------------------------------------------
 * The default size of a node's output buffer, in samples.
 * Needed because nodes such as AudioOut may be instantiated before the
 * audio hardware's sample rate is known.
 *-----------------------------------------------------------------------*/
#define SIGNAL_NODE_BUFFER_SIZE 44100

/*------------------------------------------------------------------------
 * The default trigger name, used when node->trigger() is called
 * without any parameters.
 *-----------------------------------------------------------------------*/
#define SIGNAL_DEFAULT_TRIGGER "trigger"

/*------------------------------------------------------------------------
 * Macros to aid in processing trigger inputs, for sample-rate and
 * block-rate tirggers.
 *
 * SIGNAL_CHECK_TRIGGER checks whether the specified frame of a given
 *     input has a rising edge, returning true or false
 * SIGNAL_PROCESS_TRIGGER checks whether the specified frame of a given
 *     input has a rising edge, and performs this->trigger(name) if so
 * SIGNAL_PROCESS_TRIGGER_BLOCK repeats the above operation over
 *     num_frames frames of the given input
 *-----------------------------------------------------------------------*/
#define SIGNAL_CHECK_TRIGGER(input, frame) \
	(input && input->out[0][frame] > input->out[0][frame - 1])
#define SIGNAL_PROCESS_TRIGGER(input, frame, name) \
	if (input && SIGNAL_CHECK_TRIGGER(input, frame)) { this->trigger(name); }
#define SIGNAL_PROCESS_TRIGGER_BLOCK(input, num_frames, name) \
	if (input) { for (int frame = 0; frame < num_frames; frame++) { SIGNAL_PROCESS_TRIGGER(input, frame, name); } }

typedef enum
{
	SIGNAL_SCALE_LIN_LIN,
	SIGNAL_SCALE_LIN_EXP,
	SIGNAL_SCALE_EXP_LIN,
	SIGNAL_SCALE_EXP_EXP
} signal_scale_t;

#define N_CHANNELS -1

