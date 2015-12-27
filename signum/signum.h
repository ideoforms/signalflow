void signum_init();

/*------------------------------------------------------------------------
 * Core
 *-----------------------------------------------------------------------*/
#include "constants.h"
#include "unit.h"
#include "graph.h"
#include "buffer.h"
#include "ringbuffer.h"

#include "registry.h"
#include "nodedef.h"
#include "synth.h"

/*------------------------------------------------------------------------
 * Operators
 *-----------------------------------------------------------------------*/
#include "op/multiply.h"
#include "op/divide.h"
#include "op/add.h"
#include "op/subtract.h"
#include "op/round.h"
#include "op/linexp.h"

/*------------------------------------------------------------------------
 * I/O
 *-----------------------------------------------------------------------*/
#include "io/output.h"

/*------------------------------------------------------------------------
 * Generators
 *-----------------------------------------------------------------------*/
#include "gen/constant.h"
#include "gen/sine.h"
#include "gen/square.h"
#include "gen/sampler.h"
#include "gen/granulator.h"
#include "gen/wavetable.h"
#include "gen/tick.h"
#include "gen/line.h"

/*------------------------------------------------------------------------
 * Random processes
 *-----------------------------------------------------------------------*/
#include "rnd/noise.h"
#include "rnd/dust.h"

/*------------------------------------------------------------------------
 * Envelopes
 *-----------------------------------------------------------------------*/
#include "env/env.h"

/*------------------------------------------------------------------------
 * Effects
 *-----------------------------------------------------------------------*/
#include "fx/delay.h"
#include "fx/resample.h"
#include "fx/pan.h"
#include "fx/width.h"
#include "fx/freeze.h"

/*------------------------------------------------------------------------
 * Spectral processing
 *-----------------------------------------------------------------------*/
#include "fft/fft.h"
#include "fft/ifft.h"

