/*------------------------------------------------------------------------
 * Core
 *-----------------------------------------------------------------------*/
#include <signal/core/constants.h>
#include <signal/core/core.h>
#include <signal/core/graph.h>
#include <signal/core/property.h>
#include <signal/core/random.h>
#include <signal/buffer/ringbuffer.h>
#include <signal/core/util.h>
#include <signal/core/version.h>

#include <signal/buffer/buffer.h>

#include <signal/patch/nodespec.h>
#include <signal/patch/patch.h>
#include <signal/patch/patchspec.h>

#include <signal/node/node.h>
#include <signal/node/registry.h>

/*------------------------------------------------------------------------
 * Operators
 *-----------------------------------------------------------------------*/
#include <signal/node/operators/add.h>
#include <signal/node/operators/divide.h>
#include <signal/node/operators/mixer.h>
#include <signal/node/operators/multiplex.h>
#include <signal/node/operators/multiply.h>
#include <signal/node/operators/round.h>
#include <signal/node/operators/scale.h>
#include <signal/node/operators/subtract.h>

/*------------------------------------------------------------------------
 * I/O
 *-----------------------------------------------------------------------*/
#include <signal/node/io/output/abstract.h>
#include <signal/node/io/output/ios.h>
#include <signal/node/io/output/soundio.h>

#include <signal/node/io/input/abstract.h>
#include <signal/node/io/input/soundio.h>

/*------------------------------------------------------------------------
 * Generators
 *-----------------------------------------------------------------------*/
#include <signal/node/oscillators/constant.h>
#include <signal/node/oscillators/granulator.h>
#include <signal/node/oscillators/impulse.h>
#include <signal/node/oscillators/line.h>
#include <signal/node/oscillators/saw.h>
#include <signal/node/oscillators/sine.h>
#include <signal/node/oscillators/square.h>
#include <signal/node/oscillators/triangle.h>
#include <signal/node/oscillators/wavetable.h>

/*------------------------------------------------------------------------
 * Generators
 *-----------------------------------------------------------------------*/
#include <signal/node/buffer/buffer-player.h>
#include <signal/node/buffer/buffer-recorder.h>

/*------------------------------------------------------------------------
 * Random processes
 *-----------------------------------------------------------------------*/
#include <signal/node/stochastic/logistic.h>
#include <signal/node/stochastic/noise.h>
#include <signal/node/stochastic/random-impulse.h>
#include <signal/node/stochastic/trigger-noise.h>

/*------------------------------------------------------------------------
 * Envelopes
 *-----------------------------------------------------------------------*/
#include <signal/node/envelope/adsr.h>
#include <signal/node/envelope/asr.h>

/*------------------------------------------------------------------------
 * Effects
 *-----------------------------------------------------------------------*/
#include <signal/node/filters/delays/comb.h>
#include <signal/node/filters/delays/allpass.h>
#include <signal/node/filters/eq.h>
#include <signal/node/filters/freeze.h>
#include <signal/node/filters/gate.h>
#include <signal/node/filters/moog.h>
#include <signal/node/filters/pan.h>
#include <signal/node/filters/resample.h>
#include <signal/node/filters/rms.h>
#include <signal/node/filters/waveshaper.h>
#include <signal/node/filters/width.h>

/*------------------------------------------------------------------------
 * Sequencing
 *-----------------------------------------------------------------------*/
#include <signal/node/sequencing/index.h>
#include <signal/node/sequencing/counter.h>

/*------------------------------------------------------------------------
 * Analysis and MIR
 * (In development)
 *-----------------------------------------------------------------------*/
// #include <signal/node/analysis/vamp.h>

/*------------------------------------------------------------------------
 * Control interfaces
 *-----------------------------------------------------------------------*/
#include <signal/node/control/mouse.h>

/*------------------------------------------------------------------------
 * Spectral processing
 *-----------------------------------------------------------------------*/
#ifdef __APPLE__
#include <signal/node/fft/fft.h>
#include <signal/node/fft/ifft.h>
#include <signal/node/fft/lpf.h>
#include <signal/node/fft/phase_vocoder.h>
#include <signal/node/fft/noise_gate.h>
#include <signal/node/fft/tonality.h>
#endif
