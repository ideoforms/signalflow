/*------------------------------------------------------------------------
 * Core
 *-----------------------------------------------------------------------*/
#include "core.h"
#include "version.h"
#include "constants.h"
#include "property.h"
#include "node.h"
#include "graph.h"
#include "buffer.h"
#include "ringbuffer.h"

#include "registry.h"
#include "nodedef.h"
#include "structure.h"
#include "synth.h"

/*------------------------------------------------------------------------
 * Operators
 *-----------------------------------------------------------------------*/
#include "op/multiply.h"
#include "op/divide.h"
#include "op/add.h"
#include "op/subtract.h"
#include "op/round.h"
#include "op/scale.h"
#include "op/multiplex.h"
#include "op/mixer.h"

/*------------------------------------------------------------------------
 * I/O
 *-----------------------------------------------------------------------*/
#include "io/output/abstract.h"
#include "io/output/soundio.h"
#include "io/output/ios.h"

#include "io/input/abstract.h"
#include "io/input/soundio.h"

/*------------------------------------------------------------------------
 * Generators
 *-----------------------------------------------------------------------*/
#include "gen/constant.h"
#include "gen/sine.h"
#include "gen/square.h"
#include "gen/saw.h"
#include "gen/triangle.h"
#include "gen/sampler.h"
#include "gen/recorder.h"
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
#include "fx/gate.h"
#include "fx/eq.h"
#include "fx/moog.h"
#include "fx/waveshaper.h"

/*------------------------------------------------------------------------
 * Sequencing
 *-----------------------------------------------------------------------*/
#include "seq/index.h"


/*------------------------------------------------------------------------
 * Spectral processing
 *-----------------------------------------------------------------------*/
#ifdef __APPLE__
#include "fft/fft.h"
#include "fft/ifft.h"
#include "fft/lpf.h"
#include "fft/phase_vocoder.h"
#endif


/*------------------------------------------------------------------------
 * Development headers (not included in production distribution)
 *-----------------------------------------------------------------------*/
#ifdef DEBUG
#include "signal-dev.h"
#endif

