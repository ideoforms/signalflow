#include "ringbuffer.h"

typedef float sample;
typedef sample * frame;

typedef RingBuffer <sample> SampleRingBuffer;

#define N_CHANNELS -1

#define SIGNUM_MAX_CHANNELS 32
#define SIGNUM_MAX_FFT_SIZE 8192
