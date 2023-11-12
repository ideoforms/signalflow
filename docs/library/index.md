# Node reference library

## Analysis

- **[CrossCorrelate](analysis/crosscorrelate.md)**: Outputs the cross-correlation of the input signal with the given buffer. If hop_size is zero, calculates the cross-correlation every sample.
- **[OnsetDetector](analysis/onsetdetector.md)**: Simple time-domain onset detector. Outputs an impulse when an onset is detected in the input. Maintains short-time and long-time averages. An onset is registered when the short-time average is threshold x the long-time average. min_interval is the minimum interval between onsets, in seconds.
- **[VampAnalysis](analysis/vampanalysis.md)**: Feature extraction using the Vamp plugin toolkit.

---

## Buffer

- **[BeatCutter](buffer/beatcutter.md)**: Cuts a buffer into segment_count segments, and stutters/jumps with the given probabilities.
- **[BufferLooper](buffer/bufferlooper.md)**: Read and write from a buffer concurrently, with controllable overdub.
- **[BufferPlayer](buffer/bufferplayer.md)**: Plays the contents of the given buffer. start_time/end_time are in seconds. When a clock signal is receives, rewinds to the start_time.
- **[BufferRecorder](buffer/bufferrecorder.md)**: Records the input to a buffer. feedback controls overdub.
- **[FeedbackBufferReader](buffer/feedbackbufferreader.md)**: Counterpart to FeedbackBufferWriter.
- **[FeedbackBufferWriter](buffer/feedbackbufferwriter.md)**: Counterpart to FeedbackBufferReader.
- **[GrainSegments](buffer/grainsegments.md)**: GrainSegments
- **[Granulator](buffer/granulator.md)**: Granulator. Generates a grain from the given buffer each time a clock signal is received, with the given duration/rate/pan parameters. The input buffer can be mono or stereo.
- **[SegmentPlayer](buffer/segmentplayer.md)**: Trigger segments of a buffer at the given onset positions.

---

## Control

- **[MouseX](control/mousex.md)**: Outputs the normalised cursor X position, from 0 to 1. Currently only supported on macOS.
- **[MouseY](control/mousey.md)**: Outputs the normalised cursor Y position, from 0 to 1. Currently only supported on macOS.
- **[MouseDown](control/mousedown.md)**: Outputs 1 if the left mouse button is down, 0 otherwise. Currently only supported on macOS.

---

## Envelope

- **[ADSREnvelope](envelope/adsrenvelope.md)**: Attack-decay-sustain-release envelope. Sustain portion is held until gate is zero.
- **[ASREnvelope](envelope/asrenvelope.md)**: Attack-sustain-release envelope.
- **[DetectSilence](envelope/detectsilence.md)**: Detects blocks of silence below the threshold value. Used as an auto-free node to terminate a Patch after processing is complete.
- **[Envelope](envelope/envelope.md)**: Generic envelope constructor, given an array of levels, times and curves.
- **[Line](envelope/line.md)**: Line segment with the given start/end values and duration. If loop is true, repeats indefinitely. Retriggers on a clock signal.
- **[RectangularEnvelope](envelope/rectangularenvelope.md)**: Rectangular envelope with the given sustain duration.

---

## FFT

- **[FFTContinuousPhaseVocoder](fft/fftcontinuousphasevocoder.md)**: Continuous phase vocoder. Requires an FFT* input.
- **[FFTConvolve](fft/fftconvolve.md)**: Frequency-domain convolution, using overlap-add. Useful for convolution reverb, with the input buffer containing an impulse response. Requires an FFT* input.
- **[FFT](fft/fft.md)**: Fast Fourier Transform. Takes a time-domain input, and generates a frequency-domain (FFT) output.
- **[FFTNode](fft/fftnode.md)**: FFTNode
- **[FFTOpNode](fft/fftopnode.md)**: FFTOpNode
- **[FFTFindPeaks](fft/fftfindpeaks.md)**: Find peaks in the FFT magnitude spectrum. Requires an FFT* input.
- **[IFFT](fft/ifft.md)**: Inverse Fast Fourier Transform. Requires an FFT* input, generates a time-domain output.
- **[FFTLPF](fft/fftlpf.md)**: FFT-based brick wall low pass filter. Requires an FFT* input.
- **[FFTNoiseGate](fft/fftnoisegate.md)**: FFT-based noise gate. Requires an FFT* input.
- **[FFTPhaseVocoder](fft/fftphasevocoder.md)**: Phase vocoder. Requires an FFT* input.
- **[FFTTonality](fft/ffttonality.md)**: Tonality filter. Requires an FFT* input.
- **[FFTZeroPhase](fft/fftzerophase.md)**: Remove phase information from a frequency-domain input. Requires an FFT* input.

---

## Operators

- **[Add](operators/add.md)**: Add each sample of a to each sample of b. Can also be written as a + b
- **[AmplitudeToDecibels](operators/amplitudetodecibels.md)**: Map a linear amplitude value to decibels.
- **[DecibelsToAmplitude](operators/decibelstoamplitude.md)**: DecibelsToAmplitude
- **[ChannelArray](operators/channelarray.md)**: Takes an array of inputs and spreads them across multiple channels of output.
- **[ChannelCrossfade](operators/channelcrossfade.md)**: Given a multichannel input, crossfades between channels based on the given position within the virtual array, producing a single-channel output.
- **[ChannelMixer](operators/channelmixer.md)**: Downmix a multichannel input to a lower-channel output. If num_channels is greater than one, spreads the input channels across the field. If amplitude_compensation is enabled, scale down the amplitude based on the ratio of input to output channels.
- **[ChannelSelect](operators/channelselect.md)**: Select a subset of channels from a multichannel input, starting at offset, up to a maximum of maximum, with the given step.
- **[Equal](operators/equal.md)**: Compares the output of a to the output of b. Outputs 1 when equal, 0 otherwise. Can also be written as a == b
- **[NotEqual](operators/notequal.md)**: Compares the output of a to the output of b. Outputs 0 when equal, 1 otherwise. Can also be written as a != b
- **[GreaterThan](operators/greaterthan.md)**: Compares the output of a to the output of b. Outputs 1 when a > b, 0 otherwise. Can also be written as a > b
- **[GreaterThanOrEqual](operators/greaterthanorequal.md)**: Compares the output of a to the output of b. Outputs 1 when a >= b, 0 otherwise. Can also be written as a >= b
- **[LessThan](operators/lessthan.md)**: Compares the output of a to the output of b. Outputs 1 when a < b, 0 otherwise. Can also be written as a < b
- **[LessThanOrEqual](operators/lessthanorequal.md)**: Compares the output of a to the output of b. Outputs 1 when a <= b, 0 otherwise. Can also be written as a <= b
- **[Modulo](operators/modulo.md)**: Outputs the value of a modulo b, per sample. Supports fractional values. Can also be written as a % b
- **[Abs](operators/abs.md)**: Outputs the absolute value of a, per sample. Can also be written as abs(a)
- **[If](operators/if.md)**: Outputs value_if_true for each non-zero value of a, value_if_false for all other values.
- **[Divide](operators/divide.md)**: Divide each sample of a by each sample of b. Can also be written as a / b
- **[FrequencyToMidiNote](operators/frequencytomidinote.md)**: Map a frequency to a MIDI note (where 440Hz = A4 = 69), with floating-point output.
- **[MidiNoteToFrequency](operators/midinotetofrequency.md)**: Map a MIDI note to a frequency (where 440Hz = A4 = 69), supporting floating-point input.
- **[Multiply](operators/multiply.md)**: Multiply each sample of a by each sample of b. Can also be written as a * b
- **[Pow](operators/pow.md)**: Outputs a to the power of b, per sample. Can also be written as a ** b
- **[RoundToScale](operators/roundtoscale.md)**: Given a frequency input, generates a frequency output that is rounded to the nearest MIDI note. (TODO: Not very well named)
- **[Round](operators/round.md)**: Round the input to the nearest integer value.
- **[ScaleLinExp](operators/scalelinexp.md)**: Scales the input from a linear range (between a and b) to an exponential range (between c and d).
- **[ScaleLinLin](operators/scalelinlin.md)**: Scales the input from a linear range (between a and b) to a linear range (between c and d).
- **[Subtract](operators/subtract.md)**: Subtract each sample of b from each sample of a. Can also be written as a - b
- **[Sum](operators/sum.md)**: Sums the output of all of the input nodes, by sample.
- **[Sin](operators/sin.md)**: Outputs sin(a), per sample.
- **[Cos](operators/cos.md)**: Outputs cos(a), per sample.
- **[Tan](operators/tan.md)**: Outputs tan(a), per sample.
- **[Tanh](operators/tanh.md)**: Outputs tanh(a), per sample. Can be used as a soft clipper.

---

## Oscillators

- **[Constant](oscillators/constant.md)**: Produces a constant value.
- **[Impulse](oscillators/impulse.md)**: Produces a value of 1 at the given frequency, with output of 0 at all other times. If frequency is 0, produces a single impulse.
- **[LFO](oscillators/lfo.md)**: LFO
- **[SawLFO](oscillators/sawlfo.md)**: Produces a sawtooth LFO, with output ranging from min to max.
- **[SawOscillator](oscillators/sawoscillator.md)**: Produces a (non-band-limited) sawtooth wave, with the given frequency and phase offset. When a reset or trigger is received, resets the phase to zero.
- **[SineLFO](oscillators/sinelfo.md)**: Produces a sinusoidal LFO at the given frequency and phase offset, with output ranging from min to max.
- **[SineOscillator](oscillators/sineoscillator.md)**: Produces a sine wave at the given frequency.
- **[SquareLFO](oscillators/squarelfo.md)**: Produces a pulse wave LFO with the given frequency and pulse width, ranging from min to max, where width=0.5 is a square wave.
- **[SquareOscillator](oscillators/squareoscillator.md)**: Produces a pulse wave with the given frequency and pulse width, where width=0.5 is a square wave.
- **[TriangleLFO](oscillators/trianglelfo.md)**: Produces a triangle LFO with the given frequency, ranging from min to max.
- **[TriangleOscillator](oscillators/triangleoscillator.md)**: TriangleOscillator
- **[Wavetable](oscillators/wavetable.md)**: Plays the wavetable stored in buffer at the given frequency.
- **[Wavetable2D](oscillators/wavetable2d.md)**: Wavetable2D

---

## Processors

- **[Clip](processors/clip.md)**: Clip the input to min/max.
- **[Fold](processors/fold.md)**: Fold the input beyond min/max, reflecting the excess back.
- **[Smooth](processors/smooth.md)**: Smooth the input with a given smoothing coefficient. When smooth = 0, applies no smoothing.
- **[WetDry](processors/wetdry.md)**: Takes wet and dry inputs, and outputs a mix determined by wetness.
- **[Wrap](processors/wrap.md)**: Wrap the input beyond min/max.

---

## Processors: Delays

- **[AllpassDelay](processors/delays/allpassdelay.md)**: All-pass delay, with feedback between 0 and 1. delay_time must be less than or equal to max_delay_time.
- **[CombDelay](processors/delays/combdelay.md)**: Comb delay, with feedback between 0 and 1. delay_time must be less than or equal to max_delay_time.
- **[OneTapDelay](processors/delays/onetapdelay.md)**: Single-tap delay line. delay_time must be less than or equal to max_delay_time.
- **[Stutter](processors/delays/stutter.md)**: Stutters the input whenever a signal is received on clock. Generates stutter_count repeats, with duration stutter_time.

---

## Processors: Distortion

- **[Resample](processors/distortion/resample.md)**: Resampler and bit crusher. sample_rate is in Hz, bit_rate is an integer between 0 and 16.
- **[SampleAndHold](processors/distortion/sampleandhold.md)**: Samples and holds the input each time a clock signal is received.
- **[Squiz](processors/distortion/squiz.md)**: Implementation of Dan Stowell's Squiz algorithm, a kind of downsampler.
- **[WaveShaper](processors/distortion/waveshaper.md)**: Applies wave-shaping as described in buffer.

---

## Processors: Dynamics

- **[Compressor](processors/dynamics/compressor.md)**: Dynamic range compression, with optional sidechain input.
- **[Gate](processors/dynamics/gate.md)**: Outputs the input value when it is above the given threshold, otherwise zero.
- **[Maximiser](processors/dynamics/maximiser.md)**: Gain maximiser.
- **[RMS](processors/dynamics/rms.md)**: Outputs the root-mean-squared value of the input, in buffers equal to the graph's current buffer size.

---

## Processors: Filters

- **[BiquadFilter](processors/filters/biquadfilter.md)**: Biquad filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. Not recommended for real-time modulation; for this, use SVFilter.
- **[EQ](processors/filters/eq.md)**: Three-band EQ.
- **[MoogVCF](processors/filters/moogvcf.md)**: Moog ladder low-pass filter.
- **[SVFilter](processors/filters/svfilter.md)**: State variable filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'.

---

## Processors: Panning

- **[AzimuthPanner](processors/panning/azimuthpanner.md)**: Pan input around an equally-spaced ring of num_channels speakers. pan is the pan position from -1..+1, where 0 = centre front. width is the source's width, where 1.0 spans exactly between an adjacent pair of channels.
- **[ChannelPanner](processors/panning/channelpanner.md)**: Pan the input between a linear series of channels, where pan 0 = channel 0, 1 = channel 1, etc. No wrapping is applied.
- **[SpatialPanner](processors/panning/spatialpanner.md)**: Implements a spatial panning algorithm, applied to a given SpatialEnvironment. Currently, only DBAP is supported.
- **[StereoBalance](processors/panning/stereobalance.md)**: Takes a stereo input and rebalances it, where 0 is unchanged, -1 is hard left, and 1 is hard right.
- **[StereoPanner](processors/panning/stereopanner.md)**: Pans a mono input to a stereo output. Pans from -1 (hard left) to +1 (hard right), with 0 = centre.
- **[StereoWidth](processors/panning/stereowidth.md)**: Reduces the width of a stereo signal. When width = 1, input is unchanged. When width = 0, outputs a pair of identical channels both containing L+R.

---

## Sequencing

- **[ClockDivider](sequencing/clockdivider.md)**: When given a clock input (e.g., an Impulse), divides the clock by the given factor. factor must be an integer greater than or equal to 1.
- **[Counter](sequencing/counter.md)**: Count upwards from min, driven by clock.
- **[Euclidean](sequencing/euclidean.md)**: Euclidean rhythm as described by Toussaint, with sequence_length (n) and num_events (k), driven by clock.
- **[FlipFlop](sequencing/flipflop.md)**: Flips from 0/1 on each clock.
- **[ImpulseSequence](sequencing/impulsesequence.md)**: Each time a clock or trigger is received, outputs the next value in the sequence. At all other times, outputs zero.
- **[Index](sequencing/index.md)**: Outputs the value in list corresponding to index.
- **[Latch](sequencing/latch.md)**: Initially outputs 0. When a trigger is received at set, outputs 1. When a trigger is subsequently received at reset, outputs 0, until the next set.
- **[Sequence](sequencing/sequence.md)**: Outputs the elements in sequence, incrementing position on each clock.

---

## Stochastic

- **[Logistic](stochastic/logistic.md)**: Logistic noise.
- **[PinkNoise](stochastic/pinknoise.md)**: Pink noise, with specified low/high cutoffs.
- **[RandomBrownian](stochastic/randombrownian.md)**: Outputs Brownian noise between min/max, with a mean change of delta between samples. If a clock is passed, only generates a new value on a clock tick.
- **[RandomChoice](stochastic/randomchoice.md)**: Pick a random value from the given array. If a clock is passed, only picks a new value on a clock tick.
- **[RandomCoin](stochastic/randomcoin.md)**: Flip a coin with the given probability. If a clock is passed, only picks a new value on a clock tick.
- **[RandomExponentialDist](stochastic/randomexponentialdist.md)**: Generate an random value following the exponential distribution. If a clock is passed, only picks a new value on a clock tick.
- **[RandomExponential](stochastic/randomexponential.md)**: Generate an random exponential value between min/max. If a clock is passed, only picks a new value on a clock tick.
- **[RandomGaussian](stochastic/randomgaussian.md)**: Generate an random Gaussian value, with given mean and sigma. If a clock is passed, only picks a new value on a clock tick.
- **[RandomImpulseSequence](stochastic/randomimpulsesequence.md)**: Generates a random sequence of 0/1 bits with the given length, and the given probability each each bit = 1. The position of the sequence is incremented on each clock signal. explore and generate are trigger inputs which cause the sequence to mutate and re-generate respectively.
- **[RandomImpulse](stochastic/randomimpulse.md)**: Generate random impulses at the given frequency, with either uniform or poisson distribution.
- **[RandomUniform](stochastic/randomuniform.md)**: Generates a uniformly random value between min/max. If a clock is passed, only picks a new value on a clock tick.
- **[StochasticNode](stochastic/stochasticnode.md)**: StochasticNode
- **[WhiteNoise](stochastic/whitenoise.md)**: Generates whitenoise between min/max. If frequency is zero, generates at audio rate. For frequencies lower than audio rate, interpolate applies linear interpolation between values, and random_interval specifies whether new random values should be equally-spaced or randomly-spaced.

---
