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
#define SIGNAL_DEFAULT_FFT_SIZE 8192
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

typedef enum
{
	SIGNAL_SCALE_LIN_LIN,
	SIGNAL_SCALE_LIN_EXP,
	SIGNAL_SCALE_EXP_LIN,
	SIGNAL_SCALE_EXP_EXP
} signal_scale_t;

#define N_CHANNELS -1

