# Node reference library

<!-- This code is autogenerated and should not be modified by hand. -->


## Analysis

- **[CrossCorrelate](analysis/crosscorrelate/index.md)**: Outputs the cross-correlation of the input signal with the given buffer. If hop_size is zero, calculates the cross-correlation every sample.
- **[NearestNeighbour](analysis/nearestneighbour/index.md)**: Nearest Neighbour.
- **[OnsetDetector](analysis/onsetdetector/index.md)**: Simple time-domain onset detector: outputs an impulse when an onset is detected in the input.
- **[VampAnalysis](analysis/vampanalysis/index.md)**: Feature extraction using the Vamp plugin toolkit.

---

## Buffer

- **[BeatCutter](buffer/beatcutter/index.md)**: Cuts a buffer into segment_count segments, and stutters/jumps with the given probabilities.
- **[BufferLooper](buffer/bufferlooper/index.md)**: Read and write from a buffer concurrently, with controllable overdub.
- **[BufferPlayer](buffer/bufferplayer/index.md)**: Plays the contents of the given buffer. `start_time`/`end_time` are in seconds. When a `clock` signal is received, rewinds to the `start_time`.
- **[BufferRecorder](buffer/bufferrecorder/index.md)**: Records the input to a buffer. feedback controls overdub.
- **[FeedbackBufferReader](buffer/feedbackbufferreader/index.md)**: Counterpart to FeedbackBufferWriter.
- **[FeedbackBufferWriter](buffer/feedbackbufferwriter/index.md)**: Counterpart to FeedbackBufferReader.
- **[HistoryBufferWriter](buffer/historybufferwriter/index.md)**: Writes a rolling history buffer of a given duration. At a given moment in time, the contents of the buffer will be equal to the past N seconds of the audio generated by `input`.
- **[SegmentPlayer](buffer/segmentplayer/index.md)**: Trigger segments of `buffer` at the given list of `onsets` positions, in seconds.

---

## Buffer: Granulation

- **[SegmentedGranulator](buffer/granulation/segmentedgranulator/index.md)**: Segmented Granulator.
- **[Granulator](buffer/granulation/granulator/index.md)**: Granulator. Generates a grain from the given buffer each time a trigger is received on the `clock` input. Each new grain uses the given `duration`, `amplitude`, `pan` and `rate` values presented at each input at the moment the grain is created. The input buffer can be mono or stereo. If `wrap` is true, grain playback can wrap around the end/start of the buffer.

---

## Control

- **[MouseX](control/mousex/index.md)**: Outputs the normalised cursor X position, from 0 to 1. Currently only supported on macOS.
- **[MouseY](control/mousey/index.md)**: Outputs the normalised cursor Y position, from 0 to 1. Currently only supported on macOS.
- **[MouseDown](control/mousedown/index.md)**: Outputs 1 if the left mouse button is down, 0 otherwise. Currently only supported on macOS.

---

## Envelope

- **[Accumulator](envelope/accumulator/index.md)**: Accumulator with decay.
- **[ADSREnvelope](envelope/adsrenvelope/index.md)**: Attack-decay-sustain-release envelope. Sustain portion is held until gate is zero.
- **[ASREnvelope](envelope/asrenvelope/index.md)**: Attack-sustain-release envelope.
- **[DetectSilence](envelope/detectsilence/index.md)**: Detects blocks of silence below the threshold value. Used as an auto-free node to terminate a Patch after processing is complete.
- **[Envelope](envelope/envelope/index.md)**: Generic envelope constructor, given an array of levels, times and curves.
- **[Line](envelope/line/index.md)**: Line segment with the given start/end values, and duration (in seconds). If loop is true, repeats indefinitely. Retriggers on a clock signal.
- **[RectangularEnvelope](envelope/rectangularenvelope/index.md)**: Rectangular envelope with the given sustain duration.

---

## FFT

- **[FFTContinuousPhaseVocoder](fft/fftcontinuousphasevocoder/index.md)**: Continuous phase vocoder. Requires an FFT* input.
- **[FFTConvolve](fft/fftconvolve/index.md)**: Frequency-domain convolution, using overlap-add. Useful for convolution reverb, with the input buffer containing an impulse response. Requires an FFT* input.
- **[FFTBufferPlayer](fft/fftbufferplayer/index.md)**: FFTBufferPlayer. Plays from a buffer of audio spectra in mag/phase format.
- **[FFTContrast](fft/fftcontrast/index.md)**: FFT Contrast. Requires an FFT* input.
- **[FFTCrossFade](fft/fftcrossfade/index.md)**: FFT FFTCrossFade. Requires two FFT* inputs.
- **[FFTLFO](fft/fftlfo/index.md)**: FFT LFO. Requires an FFT* input.
- **[FFTMagnitudePhaseArray](fft/fftmagnitudephasearray/index.md)**: Fixed mag/phase array.
- **[FFTRandomPhase](fft/fftrandomphase/index.md)**: Randomise phase values.
- **[FFTScaleMagnitudes](fft/fftscalemagnitudes/index.md)**: Randomise phase values.
- **[FFTTransform](fft/ffttransform/index.md)**: Transforms the FFT magnitude spectrum in the X axis. Requires an FFT* input.
- **[FFT](fft/fft/index.md)**: Fast Fourier Transform. Takes a time-domain input, and generates a frequency-domain (FFT) output.
- **[FFTFindPeaks](fft/fftfindpeaks/index.md)**: Find peaks in the FFT magnitude spectrum. Requires an FFT* input.
- **[IFFT](fft/ifft/index.md)**: Inverse Fast Fourier Transform. Requires an FFT* input, generates a time-domain output.
- **[FFTLPF](fft/fftlpf/index.md)**: FFT-based brick wall low pass filter. Requires an FFT* input.
- **[FFTNoiseGate](fft/fftnoisegate/index.md)**: FFT-based noise gate. Requires an FFT* input.
- **[FFTPhaseVocoder](fft/fftphasevocoder/index.md)**: Phase vocoder. Requires an FFT* input.
- **[FFTTonality](fft/ffttonality/index.md)**: Tonality filter. Requires an FFT* input.
- **[FFTZeroPhase](fft/fftzerophase/index.md)**: Remove phase information from a frequency-domain input. Requires an FFT* input.

---

## Operators

- **[Add](operators/add/index.md)**: Add each sample of a to each sample of b. Can also be written as a + b
- **[AmplitudeToDecibels](operators/amplitudetodecibels/index.md)**: Map a linear amplitude value to decibels.
- **[DecibelsToAmplitude](operators/decibelstoamplitude/index.md)**: DecibelsToAmplitude
- **[Bus](operators/bus/index.md)**: Bus is a node with a fixed number of input channels and arbitrary number of inputs, used to aggregate multiple sources. It is similar to Sum, but with a defined channel count that does not adapt to its inputs.
- **[ChannelArray](operators/channelarray/index.md)**: Takes an array of inputs and spreads them across multiple channels of output.
- **[ChannelCrossfade](operators/channelcrossfade/index.md)**: Given a multichannel input, crossfades between channels based on the given position within the virtual array, producing a single-channel output.
- **[ChannelMixer](operators/channelmixer/index.md)**: Downmix a multichannel input to a lower-channel output. If num_channels is greater than one, spreads the input channels across the field. If amplitude_compensation is enabled, scale down the amplitude based on the ratio of input to output channels.
- **[ChannelOffset](operators/channeloffset/index.md)**: Offsets the input by a specified number of channels. With an N-channel input and an offset of M, the output will have M+N channels.
- **[ChannelSelect](operators/channelselect/index.md)**: Select a subset of channels from a multichannel input, starting at offset, up to a maximum of maximum, with the given step.
- **[Equal](operators/equal/index.md)**: Compares the output of a to the output of b. Outputs 1 when equal, 0 otherwise. Can also be written as a == b
- **[NotEqual](operators/notequal/index.md)**: Compares the output of a to the output of b. Outputs 0 when equal, 1 otherwise. Can also be written as a != b
- **[GreaterThan](operators/greaterthan/index.md)**: Compares the output of a to the output of b. Outputs 1 when a > b, 0 otherwise. Can also be written as a > b
- **[GreaterThanOrEqual](operators/greaterthanorequal/index.md)**: Compares the output of a to the output of b. Outputs 1 when a >= b, 0 otherwise. Can also be written as a >= b
- **[LessThan](operators/lessthan/index.md)**: Compares the output of a to the output of b. Outputs 1 when a < b, 0 otherwise. Can also be written as a < b
- **[LessThanOrEqual](operators/lessthanorequal/index.md)**: Compares the output of a to the output of b. Outputs 1 when a <= b, 0 otherwise. Can also be written as a <= b
- **[Modulo](operators/modulo/index.md)**: Outputs the value of a modulo b, per sample. Supports fractional values. Can also be written as a % b
- **[Abs](operators/abs/index.md)**: Outputs the absolute value of a, per sample. Can also be written as abs(a)
- **[If](operators/if/index.md)**: Outputs value_if_true for each non-zero value of a, value_if_false for all other values.
- **[Divide](operators/divide/index.md)**: Divide each sample of a by each sample of b. Can also be written as a / b
- **[FrequencyToMidiNote](operators/frequencytomidinote/index.md)**: Map a frequency to a MIDI note (where 440Hz = A4 = 69), with floating-point output.
- **[MidiNoteToFrequency](operators/midinotetofrequency/index.md)**: Map a MIDI note to a frequency (where 440Hz = A4 = 69), supporting floating-point input.
- **[Multiply](operators/multiply/index.md)**: Multiply each sample of a by each sample of b. Can also be written as a * b
- **[Pow](operators/pow/index.md)**: Outputs a to the power of b, per sample. Can also be written as a ** b
- **[RoundToScale](operators/roundtoscale/index.md)**: Given a frequency input, generates a frequency output that is rounded to the nearest MIDI note. (TODO: Not very well named)
- **[Round](operators/round/index.md)**: Round the input to the nearest integer value.
- **[ScaleLinExp](operators/scalelinexp/index.md)**: Scales the input from a linear range (between a and b) to an exponential range (between c and d).
- **[ScaleLinLin](operators/scalelinlin/index.md)**: Scales the input from a linear range (between a and b) to a linear range (between c and d).
- **[SelectInput](operators/selectinput/index.md)**: Pass through the output of one or more `inputs`, based on the integer input index specified in `index`. Unlike `ChannelSelect`, inputs may be multichannel, and `index` can be modulated in real time.
- **[Subtract](operators/subtract/index.md)**: Subtract each sample of b from each sample of a. Can also be written as a - b
- **[Sum](operators/sum/index.md)**: Sums the output of all of the input nodes, by sample.
- **[TimeShift](operators/timeshift/index.md)**: TimeShift
- **[Sin](operators/sin/index.md)**: Outputs sin(a), per sample.
- **[Cos](operators/cos/index.md)**: Outputs cos(a), per sample.
- **[Tan](operators/tan/index.md)**: Outputs tan(a), per sample.
- **[Tanh](operators/tanh/index.md)**: Outputs tanh(a), per sample. Can be used as a soft clipper.

---

## Oscillators

- **[Impulse](oscillators/impulse/index.md)**: Produces a value of 1 at the given `frequency`, with output of 0 at all other times. If frequency is 0, produces a single impulse.
- **[SawLFO](oscillators/sawlfo/index.md)**: Produces a sawtooth LFO at the given `frequency` and `phase` offset, with output ranging from `min` to `max`.
- **[SawOscillator](oscillators/sawoscillator/index.md)**: Produces a (non-band-limited) sawtooth wave, with the given `frequency` and `phase` offset. When a `reset` or trigger is received, resets the phase to zero.
- **[SineLFO](oscillators/sinelfo/index.md)**: Produces a sinusoidal LFO at the given `frequency` and `phase` offset, with output ranging from `min` to `max`.
- **[SineOscillator](oscillators/sineoscillator/index.md)**: Produces a sine wave at the given `frequency`.
- **[SquareLFO](oscillators/squarelfo/index.md)**: Produces a pulse wave LFO with the given `frequency` and pulse `width`,  ranging from `min` to `max`, where `width` of `0.5` is a square wave and other values produce a rectangular wave.
- **[SquareOscillator](oscillators/squareoscillator/index.md)**: Produces a pulse wave with the given `frequency` and pulse `width`,  where `width` of `0.5` is a square wave and other values produce a rectangular wave.
- **[TriangleLFO](oscillators/trianglelfo/index.md)**: Produces a triangle LFO with the given `frequency` and `phase` offset, ranging from `min` to `max`.
- **[TriangleOscillator](oscillators/triangleoscillator/index.md)**: Produces a triangle wave with the given `frequency`.
- **[Wavetable](oscillators/wavetable/index.md)**: Plays the wavetable stored in buffer at the given `frequency` and `phase` offset. `sync` can be used to provide a hard sync input, which resets the wavetable's phase at each zero-crossing.
- **[Wavetable2D](oscillators/wavetable2d/index.md)**: Wavetable2D

---

## Physical

- **[Maraca](physical/maraca/index.md)**: Physically-inspired model of a maraca.

---

## Processors

- **[Clip](processors/clip/index.md)**: Clip the input to `min`/`max`.
- **[Fold](processors/fold/index.md)**: Fold the input beyond `min`/`max`, reflecting the excess back.
- **[Smooth](processors/smooth/index.md)**: Smooth the input with a given smoothing coefficient. When `smooth` = 0, applies no smoothing.
- **[WetDry](processors/wetdry/index.md)**: Takes `wet` and `dry` inputs, and outputs a mix determined by `wetness`.
- **[Wrap](processors/wrap/index.md)**: Wrap the input beyond `min`/`max`.

---

## Processors: Delays

- **[AllpassDelay](processors/delays/allpassdelay/index.md)**: All-pass delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.
- **[CombDelay](processors/delays/combdelay/index.md)**: Comb delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.
- **[OneTapDelay](processors/delays/onetapdelay/index.md)**: Single-tap delay line. `delay_time` must be less than or equal to `max_delay_time`.
- **[Stutter](processors/delays/stutter/index.md)**: Stutters the input whenever a trigger is received on `clock`. Generates `stutter_count` repeats, with duration of `stutter_time`.

---

## Processors: Distortion

- **[Resample](processors/distortion/resample/index.md)**: Resampler and bit crusher. `sample_rate` is in Hz, `bit_rate` is an integer between 0 and 16.
- **[SampleAndHold](processors/distortion/sampleandhold/index.md)**: Samples and holds the input each time a trigger is received on `clock`.
- **[Squiz](processors/distortion/squiz/index.md)**: Implementation of Dan Stowell's Squiz algorithm, a kind of downsampler.
- **[WaveShaper](processors/distortion/waveshaper/index.md)**: Applies wave-shaping as described in the WaveShaperBuffer `buffer`.

---

## Processors: Dynamics

- **[Compressor](processors/dynamics/compressor/index.md)**: Dynamic range compression, with optional `sidechain` input. When the input amplitude is above `threshold`, compresses the amplitude with the given `ratio`, following the given `attack_time` and `release_time` in seconds.
- **[Gate](processors/dynamics/gate/index.md)**: Outputs the input value when it is above the given `threshold`, otherwise zero.
- **[Maximiser](processors/dynamics/maximiser/index.md)**: Gain maximiser.
- **[RMS](processors/dynamics/rms/index.md)**: Outputs the root-mean-squared value of the input, in buffers equal to the graph's current buffer size.

---

## Processors: Filters

- **[BiquadFilter](processors/filters/biquadfilter/index.md)**: Biquad filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. Not recommended for real-time modulation; for this, use SVFilter.
- **[DCFilter](processors/filters/dcfilter/index.md)**: Remove low-frequency and DC content from a signal.
- **[EQ](processors/filters/eq/index.md)**: Three-band EQ.
- **[MoogVCF](processors/filters/moogvcf/index.md)**: Simulation of the Moog ladder low-pass filter. `cutoff` sets the cutoff frequency; `resonance` should typically be between 0..1.
- **[SVFilter](processors/filters/svfilter/index.md)**: State variable filter. `filter_type` can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. `resonance` should be between `[0..1]`.

---

## Processors: Panning

- **[AzimuthPanner](processors/panning/azimuthpanner/index.md)**: Pan input around an equally-spaced ring of `num_channels` speakers. `pan` is the pan position from -1..+1, where 0 = centre front. `width` is the source's width, where 1.0 spans exactly between an adjacent pair of channels.
- **[ChannelPanner](processors/panning/channelpanner/index.md)**: Pan the input between a linear series of channels, where `pan` 0 = channel 0, 1 = channel 1, etc. No wrapping is applied.
- **[SpatialPanner](processors/panning/spatialpanner/index.md)**: Implements a spatial panning algorithm, applied to a given SpatialEnvironment. Currently, only DBAP is supported.
- **[StereoBalance](processors/panning/stereobalance/index.md)**: Takes a stereo input and rebalances it, where `balance` of `0` is unchanged, `-1` is hard left, and `1` is hard right.
- **[StereoPanner](processors/panning/stereopanner/index.md)**: Pans a mono input to a stereo output. `pan` should be between -1 (hard left) to +1 (hard right), with 0 = centre.
- **[StereoWidth](processors/panning/stereowidth/index.md)**: Reduces the width of a stereo signal. When `width` = 1, input is unchanged. When `width` = 0, outputs a pair of identical channels both containing L+R.

---

## Sequencing

- **[ClockDivider](sequencing/clockdivider/index.md)**: When given a `clock` input (e.g., an Impulse), divides the clock by the given `factor`. factor must be an integer greater than or equal to 1.
- **[Counter](sequencing/counter/index.md)**: Count upwards from `min` to `max`, driven by `clock`.
- **[Euclidean](sequencing/euclidean/index.md)**: Euclidean rhythm as described by Toussaint, with `sequence_length` (n) and `num_events` (k), driven by `clock`.
- **[FlipFlop](sequencing/flipflop/index.md)**: Flips from 0/1 on each `clock`.
- **[ImpulseSequence](sequencing/impulsesequence/index.md)**: Each time a `clock` or trigger is received, outputs the next value in `sequence`. At all other times, outputs zero.
- **[Index](sequencing/index/index.md)**: Outputs the value in `list` corresponding to `index`.
- **[Latch](sequencing/latch/index.md)**: Initially outputs 0. When a trigger is received at `set`, outputs 1. When a trigger is subsequently received at `reset`, outputs 0, until the next `set`.
- **[Sequence](sequencing/sequence/index.md)**: Outputs the elements in `sequence`, incrementing position on each `clock`.
- **[TriggerMult](sequencing/triggermult/index.md)**: Distribute any triggers to all output nodes.
- **[TriggerRoundRobin](sequencing/triggerroundrobin/index.md)**: Relay trigger() events to a single node from the list of connected outputs, with `direction` determining the direction: 1 (or above) = move forwards by N, -1 = move backwards by N, 0 = stationary.

---

## Stochastic

- **[Logistic](stochastic/logistic/index.md)**: Logistic noise.
- **[PinkNoise](stochastic/pinknoise/index.md)**: Pink noise, with specified low/high cutoffs.
- **[RandomBrownian](stochastic/randombrownian/index.md)**: Outputs Brownian noise between min/max, with a mean change of delta between samples. If a clock is passed, only generates a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomChoice](stochastic/randomchoice/index.md)**: Pick a random value from the given array. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomCoin](stochastic/randomcoin/index.md)**: Flip a coin with the given probability. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomExponentialDist](stochastic/randomexponentialdist/index.md)**: Generate an random value following the exponential distribution. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomExponential](stochastic/randomexponential/index.md)**: Generate an random exponential value between min/max. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomGaussian](stochastic/randomgaussian/index.md)**: Generate an random Gaussian value, with given mean and sigma. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomImpulseSequence](stochastic/randomimpulsesequence/index.md)**: Generates a random sequence of 0/1 bits with the given length, and the given probability each each bit = 1. The position of the sequence is incremented on each clock signal. explore and generate are trigger inputs which cause the sequence to mutate and re-generate respectively.
- **[RandomImpulse](stochastic/randomimpulse/index.md)**: Generate random impulses at the given frequency, with either uniform or poisson distribution.
- **[RandomUniform](stochastic/randomuniform/index.md)**: Generates a uniformly random value between min/max. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[WhiteNoise](stochastic/whitenoise/index.md)**: Generates whitenoise between min/max. If frequency is zero, generates at audio rate. For frequencies lower than audio rate, interpolate applies linear interpolation between values, and random_interval specifies whether new random values should be equally-spaced or randomly-spaced.

---
