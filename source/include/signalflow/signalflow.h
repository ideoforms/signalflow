/*------------------------------------------------------------------------
 * Core
 *-----------------------------------------------------------------------*/
#include <signalflow/core/config.h>
#include <signalflow/core/constants.h>
#include <signalflow/core/core.h>
#include <signalflow/core/exceptions.h>
#include <signalflow/core/graph.h>
#include <signalflow/core/kdtree.h>
#include <signalflow/core/property.h>
#include <signalflow/core/random.h>
#include <signalflow/core/util.h>
#include <signalflow/core/version.h>

/*------------------------------------------------------------------------
 * Node
 *-----------------------------------------------------------------------*/
#include <signalflow/node/node.h>
#include <signalflow/node/registry.h>

/*------------------------------------------------------------------------
 * Patch
 *-----------------------------------------------------------------------*/
#include <signalflow/patch/patch-node-spec.h>
#include <signalflow/patch/patch-registry.h>
#include <signalflow/patch/patch-spec.h>
#include <signalflow/patch/patch.h>

/*------------------------------------------------------------------------
 * Buffers
 *-----------------------------------------------------------------------*/
#include <signalflow/buffer/buffer.h>
#include <signalflow/buffer/envelope-buffer.h>
#include <signalflow/buffer/fftbuffer.h>
#include <signalflow/buffer/ringbuffer.h>
#include <signalflow/buffer/waveshaper-buffer.h>
#include <signalflow/buffer/wavetable-buffer.h>

/*------------------------------------------------------------------------
 * Operators
 *-----------------------------------------------------------------------*/
#include <signalflow/node/operators/add.h>
#include <signalflow/node/operators/amplitude-to-decibels.h>
#include <signalflow/node/operators/channel-array.h>
#include <signalflow/node/operators/channel-crossfade.h>
#include <signalflow/node/operators/channel-mixer.h>
#include <signalflow/node/operators/channel-select.h>
#include <signalflow/node/operators/comparison.h>
#include <signalflow/node/operators/divide.h>
#include <signalflow/node/operators/frequency-to-midi-note.h>
#include <signalflow/node/operators/midi-note-to-frequency.h>
#include <signalflow/node/operators/multiply.h>
#include <signalflow/node/operators/pow.h>
#include <signalflow/node/operators/round-to-scale.h>
#include <signalflow/node/operators/round.h>
#include <signalflow/node/operators/scale.h>
#include <signalflow/node/operators/subtract.h>
#include <signalflow/node/operators/sum.h>
#include <signalflow/node/operators/time-shift.h>
#include <signalflow/node/operators/trigonometry.h>

/*------------------------------------------------------------------------
 * I/O
 *-----------------------------------------------------------------------*/
#include <signalflow/node/io/output/abstract.h>
#include <signalflow/node/io/output/dummy.h>
#include <signalflow/node/io/output/ios.h>
#include <signalflow/node/io/output/soundio.h>

#include <signalflow/node/io/input/abstract.h>
#include <signalflow/node/io/input/soundio.h>

/*------------------------------------------------------------------------
 * Oscillators
 *-----------------------------------------------------------------------*/
#include <signalflow/node/oscillators/constant.h>
#include <signalflow/node/oscillators/impulse.h>
#include <signalflow/node/oscillators/saw-lfo.h>
#include <signalflow/node/oscillators/saw.h>
#include <signalflow/node/oscillators/sine-lfo.h>
#include <signalflow/node/oscillators/sine.h>
#include <signalflow/node/oscillators/square-lfo.h>
#include <signalflow/node/oscillators/square.h>
#include <signalflow/node/oscillators/triangle-lfo.h>
#include <signalflow/node/oscillators/triangle.h>
#include <signalflow/node/oscillators/wavetable.h>

/*------------------------------------------------------------------------
 * Buffer players
 *-----------------------------------------------------------------------*/
#include <signalflow/node/buffer/beat-cutter.h>
#include <signalflow/node/buffer/buffer-looper.h>
#include <signalflow/node/buffer/buffer-player.h>
#include <signalflow/node/buffer/buffer-recorder.h>
#include <signalflow/node/buffer/feedback-buffer-reader.h>
#include <signalflow/node/buffer/feedback-buffer-writer.h>
#include <signalflow/node/buffer/granulation/grainsegments.h>
#include <signalflow/node/buffer/granulation/granulator.h>
#include <signalflow/node/buffer/segment-player.h>

/*------------------------------------------------------------------------
 * Random processes
 *-----------------------------------------------------------------------*/
#include <signalflow/node/stochastic/logistic.h>
#include <signalflow/node/stochastic/pink-noise.h>
#include <signalflow/node/stochastic/random-brownian.h>
#include <signalflow/node/stochastic/random-choice.h>
#include <signalflow/node/stochastic/random-coin.h>
#include <signalflow/node/stochastic/random-exponential-dist.h>
#include <signalflow/node/stochastic/random-exponential.h>
#include <signalflow/node/stochastic/random-gaussian.h>
#include <signalflow/node/stochastic/random-impulse-sequence.h>
#include <signalflow/node/stochastic/random-impulse.h>
#include <signalflow/node/stochastic/random-uniform.h>
#include <signalflow/node/stochastic/white-noise.h>

/*------------------------------------------------------------------------
 * Envelopes and lifecycle
 *-----------------------------------------------------------------------*/
#include <signalflow/node/envelope/adsr.h>
#include <signalflow/node/envelope/asr.h>
#include <signalflow/node/envelope/detect-silence.h>
#include <signalflow/node/envelope/envelope.h>
#include <signalflow/node/envelope/line.h>
#include <signalflow/node/envelope/rect.h>

/*------------------------------------------------------------------------
 * Processors
 *-----------------------------------------------------------------------*/
#include <signalflow/node/processors/clip.h>
#include <signalflow/node/processors/delays/allpass.h>
#include <signalflow/node/processors/delays/comb.h>
#include <signalflow/node/processors/delays/onetap.h>
#include <signalflow/node/processors/delays/stutter.h>
#include <signalflow/node/processors/distortion/resample.h>
#include <signalflow/node/processors/distortion/sample-and-hold.h>
#include <signalflow/node/processors/distortion/squiz.h>
#include <signalflow/node/processors/distortion/waveshaper.h>
#include <signalflow/node/processors/dynamics/compressor.h>
#include <signalflow/node/processors/dynamics/gate.h>
#include <signalflow/node/processors/dynamics/maximiser.h>
#include <signalflow/node/processors/dynamics/rms.h>
#include <signalflow/node/processors/filters/biquad.h>
#include <signalflow/node/processors/filters/dc.h>
#include <signalflow/node/processors/filters/eq.h>
#include <signalflow/node/processors/filters/moog.h>
#include <signalflow/node/processors/filters/svf.h>
#include <signalflow/node/processors/fold.h>
#include <signalflow/node/processors/panning/azimuth-panner.h>
#include <signalflow/node/processors/panning/channel-panner.h>
#include <signalflow/node/processors/panning/spatial-environment.h>
#include <signalflow/node/processors/panning/stereo-balance.h>
#include <signalflow/node/processors/panning/stereo-panner.h>
#include <signalflow/node/processors/panning/stereo-width.h>
#include <signalflow/node/processors/smooth.h>
#include <signalflow/node/processors/wetdry.h>
#include <signalflow/node/processors/wrap.h>

/*------------------------------------------------------------------------
 * Sequencing
 *-----------------------------------------------------------------------*/
#include <signalflow/node/sequencing/clock-divider.h>
#include <signalflow/node/sequencing/counter.h>
#include <signalflow/node/sequencing/euclidean.h>
#include <signalflow/node/sequencing/flipflop.h>
#include <signalflow/node/sequencing/impulse-sequence.h>
#include <signalflow/node/sequencing/index.h>
#include <signalflow/node/sequencing/latch.h>
#include <signalflow/node/sequencing/sequence.h>
#include <signalflow/node/sequencing/trigger-mult.h>
#include <signalflow/node/sequencing/trigger-round-robin.h>

/*------------------------------------------------------------------------
 * Analysis and MIR
 *-----------------------------------------------------------------------*/
#include <signalflow/node/analysis/cross-correlate.h>
#include <signalflow/node/analysis/nearest-neighbour.h>
#include <signalflow/node/analysis/onset-detector.h>
#include <signalflow/node/analysis/vamp.h>

/*------------------------------------------------------------------------
 * Control interfaces
 *-----------------------------------------------------------------------*/
#include <signalflow/node/control/mouse.h>

/*------------------------------------------------------------------------
 * FFT
 *-----------------------------------------------------------------------*/
#include <signalflow/node/fft/continuous-pv.h>
#include <signalflow/node/fft/fft-buffer-player.h>
#include <signalflow/node/fft/fft-contrast.h>
#include <signalflow/node/fft/fft-cross-fade.h>
#include <signalflow/node/fft/fft-lfo.h>
#include <signalflow/node/fft/fft-magnitude-phase-array.h>
#include <signalflow/node/fft/fft-random-phase.h>
#include <signalflow/node/fft/fft-scale-magnitudes.h>
#include <signalflow/node/fft/fft-transform.h>
#include <signalflow/node/fft/fft.h>
#include <signalflow/node/fft/find-peaks.h>
#include <signalflow/node/fft/ifft.h>
#include <signalflow/node/fft/lpf.h>
#include <signalflow/node/fft/noise-gate.h>
#include <signalflow/node/fft/phase-vocoder.h>
#include <signalflow/node/fft/tonality.h>

#ifdef __APPLE__
#include <signalflow/node/fft/convolve.h>
#endif
