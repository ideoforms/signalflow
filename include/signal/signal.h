/*------------------------------------------------------------------------
 * Core
 *-----------------------------------------------------------------------*/
#include "signal/core/core.h"
#include "signal/core/version.h"
#include "signal/core/constants.h"
#include "signal/core/property.h"
#include "signal/core/graph.h"
#include "signal/core/ringbuffer.h"

#include "signal/buffer/buffer.h"

#include "signal/synth/nodedef.h"
#include "signal/synth/synthspec.h"
#include "signal/synth/synthtemplate.h"
#include "signal/synth/synth.h"

#include "signal/node/node.h"
#include "signal/node/registry.h"


/*------------------------------------------------------------------------
 * Operators
 *-----------------------------------------------------------------------*/
#include "signal/node/operators/multiply.h"
#include "signal/node/operators/divide.h"
#include "signal/node/operators/add.h"
#include "signal/node/operators/subtract.h"
#include "signal/node/operators/round.h"
#include "signal/node/operators/scale.h"
#include "signal/node/operators/multiplex.h"
#include "signal/node/operators/mixer.h"

/*------------------------------------------------------------------------
 * I/O
 *-----------------------------------------------------------------------*/
#include "signal/node/io/output/abstract.h"
#include "signal/node/io/output/soundio.h"
#include "signal/node/io/output/ios.h"

#include "signal/node/io/input/abstract.h"
#include "signal/node/io/input/soundio.h"

/*------------------------------------------------------------------------
 * Generators
 *-----------------------------------------------------------------------*/
#include "node/oscillators/constant.h"
#include "node/oscillators/sine.h"
#include "node/oscillators/square.h"
#include "node/oscillators/saw.h"
#include "node/oscillators/triangle.h"
#include "node/oscillators/sampler.h"
#include "node/oscillators/recorder.h"
#include "node/oscillators/granulator.h"
#include "node/oscillators/wavetable.h"
#include "node/oscillators/tick.h"
#include "node/oscillators/line.h"

/*------------------------------------------------------------------------
 * Random processes
 *-----------------------------------------------------------------------*/
#include "node/chance/noise.h"
#include "node/chance/logistic.h"
#include "node/chance/trigger_noise.h"
#include "node/chance/dust.h"

/*------------------------------------------------------------------------
 * Envelopes
 *-----------------------------------------------------------------------*/
#include "node/envelope/env.h"

/*------------------------------------------------------------------------
 * Effects
 *-----------------------------------------------------------------------*/
#include "node/filters/delay.h"
#include "node/filters/resample.h"
#include "node/filters/pan.h"
#include "node/filters/width.h"
#include "node/filters/freeze.h"
#include "node/filters/gate.h"
#include "node/filters/eq.h"
#include "node/filters/moog.h"
#include "node/filters/waveshaper.h"

/*------------------------------------------------------------------------
 * Sequencing
 *-----------------------------------------------------------------------*/
#include "node/sequencing/index.h"

/*------------------------------------------------------------------------
 * Analysis and MIR
 * (In development)
 *-----------------------------------------------------------------------*/
// #include "analysis/vamp.h"

/*------------------------------------------------------------------------
 * Control interfaces
 *-----------------------------------------------------------------------*/
#include "node/control/mouse.h"

/*------------------------------------------------------------------------
 * Spectral processing
 *-----------------------------------------------------------------------*/
#ifdef __APPLE__
#include "node/fft/fft.h"
#include "node/fft/ifft.h"
#include "node/fft/lpf.h"
#include "node/fft/phase_vocoder.h"
#endif
