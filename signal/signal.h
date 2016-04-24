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
#include "operators/multiply.h"
#include "operators/divide.h"
#include "operators/add.h"
#include "operators/subtract.h"
#include "operators/round.h"
#include "operators/scale.h"
#include "operators/multiplex.h"
#include "operators/mixer.h"

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
#include "oscillators/constant.h"
#include "oscillators/sine.h"
#include "oscillators/square.h"
#include "oscillators/saw.h"
#include "oscillators/triangle.h"
#include "oscillators/sampler.h"
#include "oscillators/recorder.h"
#include "oscillators/granulator.h"
#include "oscillators/wavetable.h"
#include "oscillators/tick.h"
#include "oscillators/line.h"

/*------------------------------------------------------------------------
 * Random processes
 *-----------------------------------------------------------------------*/
#include "chance/noise.h"
#include "chance/dust.h"

/*------------------------------------------------------------------------
 * Envelopes
 *-----------------------------------------------------------------------*/
#include "envelope/env.h"

/*------------------------------------------------------------------------
 * Effects
 *-----------------------------------------------------------------------*/
#include "filters/delay.h"
#include "filters/resample.h"
#include "filters/pan.h"
#include "filters/width.h"
#include "filters/freeze.h"
#include "filters/gate.h"
#include "filters/eq.h"
#include "filters/moog.h"
#include "filters/waveshaper.h"

/*------------------------------------------------------------------------
 * Sequencing
 *-----------------------------------------------------------------------*/
#include "sequencing/index.h"


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

