# Node reference library


## Analysis

- **CrossCorrelate**: Outputs the cross-correlation of the input signal with the given buffer. If hop_size is zero, calculates the cross-correlation every sample. `(input=nullptr, buffer=nullptr, hop_size=0)`
- **OnsetDetector**: Simple time-domain onset detector. Outputs an impulse when an onset is detected in the input. Maintains short-time and long-time averages. An onset is registered when the short-time average is threshold x the long-time average. min_interval is the minimum interval between onsets, in seconds. `(input=0.0, threshold=2.0, min_interval=0.1)`
- **VampAnalysis**: Feature extraction using the Vamp plugin toolkit. `(input=0.0, plugin_id="vamp-example-plugins:spectralcentroid:linearcentroid")`

## Buffer

- **BeatCutter**: Cuts a buffer into segment_count segments, and stutters/jumps with the given probabilities. `(buffer=nullptr, segment_count=8, stutter_probability=0.0, stutter_count=1, jump_probability=0.0, duty_cycle=1.0, rate=1.0, segment_rate=1.0)`
- **BufferLooper**: Read and write from a buffer concurrently, with controllable overdub. `(buffer=nullptr, input=0.0, feedback=0.0, loop_playback=false, loop_record=false)`
- **BufferPlayer**: Plays the contents of the given buffer. start_time/end_time are in seconds. When a clock signal is receives, rewinds to the start_time. `(buffer=nullptr, rate=1.0, loop=0, start_time=nullptr, end_time=nullptr, clock=nullptr)`
- **BufferRecorder**: Records the input to a buffer. feedback controls overdub. `(buffer=nullptr, input=0.0, feedback=0.0, loop=false)`
- **FeedbackBufferReader**: Counterpart to FeedbackBufferWriter. `(buffer=nullptr)`
- **FeedbackBufferWriter**: Counterpart to FeedbackBufferReader. `(buffer=nullptr, input=0.0, delay_time=0.1)`
- **GrainSegments**: GrainSegments `(buffer=nullptr, clock=0, target=0, offsets={}, values={}, durations={})`
- **Granulator**: Granulator. Generates a grain from the given buffer each time a clock signal is received, with the given duration/rate/pan parameters. The input buffer can be mono or stereo. `(buffer=nullptr, clock=0, pos=0, duration=0.1, pan=0.0, rate=1.0, max_grains=2048)`
- **SegmentPlayer**: Trigger segments of a buffer at the given onset positions. `(buffer=nullptr, onsets={})`

## Control

- **MouseX**: Outputs the normalised cursor X position, from 0 to 1. Currently only supported on macOS. `()`
- **MouseY**: Outputs the normalised cursor Y position, from 0 to 1. Currently only supported on macOS. `()`
- **MouseDown**: Outputs 1 if the left mouse button is down, 0 otherwise. Currently only supported on macOS. `(button_index=0)`

## Envelope

- **ADSREnvelope**: Attack-decay-sustain-release envelope. Sustain portion is held until gate is zero. `(attack=0.1, decay=0.1, sustain=0.5, release=0.1, gate=0)`
- **ASREnvelope**: Attack-sustain-release envelope. `(attack=0.1, sustain=0.5, release=0.1, curve=1.0, clock=nullptr)`
- **DetectSilence**: Detects blocks of silence below the threshold value. Used as an auto-free node to terminate a Patch after processing is complete. `(input=nullptr, threshold=0.00001)`
- **Envelope**: Generic envelope constructor, given an array of levels, times and curves. `(levels=std::vector<NodeRef> ( ), times=std::vector<NodeRef> ( ), curves=std::vector<NodeRef> ( ), clock=nullptr, loop=false)`
- **Line**: Line segment with the given start/end values and duration. If loop is true, repeats indefinitely. Retriggers on a clock signal. `(from=0.0, to=1.0, time=1.0, loop=0, clock=nullptr)`
- **RectangularEnvelope**: Rectangular envelope with the given sustain duration. `(sustain_duration=1.0, clock=nullptr)`

## Fft

- **FFTContinuousPhaseVocoder**: Continuous phase vocoder. Requires an FFT* input. `(input=nullptr, rate=1.0)`
- **FFTConvolve**: Frequency-domain convolution, using overlap-add. Useful for convolution reverb, with the input buffer containing an impulse response. Requires an FFT* input. `(input=nullptr, buffer=nullptr)`
- **FFT**: Fast Fourier Transform. Takes a time-domain input, and generates a frequency-domain (FFT) output. `(input=0.0, fft_size=SIGNALFLOW_DEFAULT_FFT_SIZE, hop_size=SIGNALFLOW_DEFAULT_FFT_HOP_SIZE, window_size=0, do_window=true)`
- **FFTNode**: FFTNode `(fft_size=None, hop_size=None, window_size=None, do_window=None)`
- **FFTOpNode**: FFTOpNode `(input=nullptr)`
- **FFTFindPeaks**: Find peaks in the FFT magnitude spectrum. Requires an FFT* input. `(input=0, prominence=1, threshold=0.000001, count=SIGNALFLOW_MAX_CHANNELS, interpolate=true)`
- **IFFT**: Inverse Fast Fourier Transform. Requires an FFT* input, generates a time-domain output. `(input=nullptr, do_window=false)`
- **FFTLPF**: FFT-based brick wall low pass filter. Requires an FFT* input. `(input=0, frequency=2000)`
- **FFTNoiseGate**: FFT-based noise gate. Requires an FFT* input. `(input=0, threshold=0.5)`
- **FFTPhaseVocoder**: Phase vocoder. Requires an FFT* input. `(input=nullptr)`
- **FFTTonality**: Tonality filter. Requires an FFT* input. `(input=0, level=0.5, smoothing=0.9)`
- **FFTZeroPhase**: Remove phase information from a frequency-domain input. Requires an FFT* input. `(input=0)`

## Operators

- **Add**: Add each sample of a to each sample of b. Can also be written as a + b `(a=0, b=0)`
- **AmplitudeToDecibels**: Map a linear amplitude value to decibels. `(a=0)`
- **DecibelsToAmplitude**: DecibelsToAmplitude `(a=0)`
- **ChannelArray**: Takes an array of inputs and spreads them across multiple channels of output. `()`
- **ChannelCrossfade**: Given a multichannel input, crossfades between channels based on the given position within the virtual array, producing a single-channel output. `(input=nullptr, index=nullptr, num_output_channels=1)`
- **ChannelMixer**: Downmix a multichannel input to a lower-channel output. If num_channels is greater than one, spreads the input channels across the field. If amplitude_compensation is enabled, scale down the amplitude based on the ratio of input to output channels. `(num_channels=1, input=0, amplitude_compensation=true)`
- **ChannelSelect**: Select a subset of channels from a multichannel input, starting at offset, up to a maximum of maximum, with the given step. `(input=nullptr, offset=0, maximum=0, step=1)`
- **Equal**: Compares the output of a to the output of b. Outputs 1 when equal, 0 otherwise. Can also be written as a == b `(a=0, b=0)`
- **NotEqual**: Compares the output of a to the output of b. Outputs 0 when equal, 1 otherwise. Can also be written as a != b `(a=0, b=0)`
- **GreaterThan**: Compares the output of a to the output of b. Outputs 1 when a > b, 0 otherwise. Can also be written as a > b `(a=0, b=0)`
- **GreaterThanOrEqual**: Compares the output of a to the output of b. Outputs 1 when a >= b, 0 otherwise. Can also be written as a >= b `(a=0, b=0)`
- **LessThan**: Compares the output of a to the output of b. Outputs 1 when a < b, 0 otherwise. Can also be written as a < b `(a=0, b=0)`
- **LessThanOrEqual**: Compares the output of a to the output of b. Outputs 1 when a <= b, 0 otherwise. Can also be written as a <= b `(a=0, b=0)`
- **Modulo**: Outputs the value of a modulo b, per sample. Supports fractional values. Can also be written as a % b `(a=0, b=0)`
- **Abs**: Outputs the absolute value of a, per sample. Can also be written as abs(a) `(a=0)`
- **If**: Outputs value_if_true for each non-zero value of a, value_if_false for all other values. `(a=0, value_if_true=0, value_if_false=0)`
- **Divide**: Divide each sample of a by each sample of b. Can also be written as a / b `(a=1, b=1)`
- **FrequencyToMidiNote**: Map a frequency to a MIDI note (where 440Hz = A4 = 69), with floating-point output. `(a=0)`
- **MidiNoteToFrequency**: Map a MIDI note to a frequency (where 440Hz = A4 = 69), supporting floating-point input. `(a=0)`
- **Multiply**: Multiply each sample of a by each sample of b. Can also be written as a * b `(a=1.0, b=1.0)`
- **Pow**: Outputs a to the power of b, per sample. Can also be written as a ** b `(a=0, b=0)`
- **RoundToScale**: Given a frequency input, generates a frequency output that is rounded to the nearest MIDI note. (TODO: Not very well named) `(a=0)`
- **Round**: Round the input to the nearest integer value. `(a=0)`
- **ScaleLinExp**: Scales the input from a linear range (between a and b) to an exponential range (between c and d). `(input=0, a=0, b=1, c=1, d=10)`
- **ScaleLinLin**: Scales the input from a linear range (between a and b) to a linear range (between c and d). `(input=0, a=0, b=1, c=1, d=10)`
- **Subtract**: Subtract each sample of b from each sample of a. Can also be written as a - b `(a=0, b=0)`
- **Sum**: Sums the output of all of the input nodes, by sample. `()`
- **Sin**: Outputs sin(a), per sample. `(a=0)`
- **Cos**: Outputs cos(a), per sample. `(a=0)`
- **Tan**: Outputs tan(a), per sample. `(a=0)`
- **Tanh**: Outputs tanh(a), per sample. Can be used as a soft clipper. `(a=0)`

## Oscillators

- **Constant**: Produces a constant value. `(value=0)`
- **Impulse**: Produces a value of 1 at the given frequency, with output of 0 at all other times. If frequency is 0, produces a single impulse. `(frequency=1.0)`
- **LFO**: LFO `(frequency=1.0, min=0.0, max=1.0, phase=0.0)`
- **SawLFO**: Produces a sawtooth LFO, with output ranging from min to max. `(frequency=1.0, min=0.0, max=1.0, phase=0.0)`
- **SawOscillator**: Produces a (non-band-limited) sawtooth wave, with the given frequency and phase offset. When a reset or trigger is received, resets the phase to zero. `(frequency=440, phase=nullptr, reset=nullptr)`
- **SineLFO**: Produces a sinusoidal LFO at the given frequency and phase offset, with output ranging from min to max. `(frequency=1.0, min=0.0, max=1.0, phase=0.0)`
- **SineOscillator**: Produces a sine wave at the given frequency. `(frequency=440)`
- **SquareLFO**: Produces a pulse wave LFO with the given frequency and pulse width, ranging from min to max, where width=0.5 is a square wave. `(frequency=1.0, min=0.0, max=1.0, width=0.5, phase=0.0)`
- **SquareOscillator**: Produces a pulse wave with the given frequency and pulse width, where width=0.5 is a square wave. `(frequency=440, width=0.5)`
- **TriangleLFO**: Produces a triangle LFO with the given frequency, ranging from min to max. `(frequency=1.0, min=0.0, max=1.0, phase=0.0)`
- **TriangleOscillator**: TriangleOscillator `(frequency=440)`
- **Wavetable**: Plays the wavetable stored in buffer at the given frequency. `(buffer=nullptr, frequency=440, phase=0, sync=0, phase_map=nullptr)`
- **Wavetable2D**: Wavetable2D `(buffer=nullptr, frequency=440, crossfade=0.0, phase=0.0, sync=0)`

## Processors

- **Clip**: Clip the input to min/max. `(input=nullptr, min=-1.0, max=1.0)`
- **Fold**: Fold the input beyond min/max, reflecting the excess back. `(input=nullptr, min=-1.0, max=1.0)`
- **Smooth**: Smooth the input with a given smoothing coefficient. When smooth = 0, applies no smoothing. `(input=nullptr, smooth=0.99)`
- **WetDry**: Takes wet and dry inputs, and outputs a mix determined by wetness. `(dry_input=nullptr, wet_input=nullptr, wetness=0.0)`
- **Wrap**: Wrap the input beyond min/max. `(input=nullptr, min=-1.0, max=1.0)`

## Processors: Delays

- **AllpassDelay**: All-pass delay, with feedback between 0 and 1. delay_time must be less than or equal to max_delay_time. `(input=0.0, delay_time=0.1, feedback=0.5, max_delay_time=0.5)`
- **CombDelay**: Comb delay, with feedback between 0 and 1. delay_time must be less than or equal to max_delay_time. `(input=0.0, delay_time=0.1, feedback=0.5, max_delay_time=0.5)`
- **OneTapDelay**: Single-tap delay line. delay_time must be less than or equal to max_delay_time. `(input=0.0, delay_time=0.1, max_delay_time=0.5)`
- **Stutter**: Stutters the input whenever a signal is received on clock. Generates stutter_count repeats, with duration stutter_time. `(input=0.0, stutter_time=0.1, stutter_count=1, clock=nullptr, max_stutter_time=1.0)`

## Processors: Distortion

- **Resample**: Resampler and bit crusher. sample_rate is in Hz, bit_rate is an integer between 0 and 16. `(input=0, sample_rate=44100, bit_rate=16)`
- **SampleAndHold**: Samples and holds the input each time a clock signal is received. `(input=nullptr, clock=nullptr)`
- **Squiz**: Implementation of Dan Stowell's Squiz algorithm, a kind of downsampler. `(input=0.0, rate=2.0, chunk_size=1)`
- **WaveShaper**: Applies wave-shaping as described in buffer. `(input=0.0, buffer=nullptr)`

## Processors: Dynamics

- **Compressor**: Dynamic range compression, with optional sidechain input. `(input=0.0, threshold=0.1, ratio=2, attack_time=0.01, release_time=0.1, sidechain=nullptr)`
- **Gate**: Outputs the input value when it is above the given threshold, otherwise zero. `(input=0.0, threshold=0.1)`
- **Maximiser**: Gain maximiser. `(input=0.0, ceiling=0.5, attack_time=1.0, release_time=1.0)`
- **RMS**: Outputs the root-mean-squared value of the input, in buffers equal to the graph's current buffer size. `(input=0.0)`

## Processors: Filters

- **BiquadFilter**: Biquad filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. Not recommended for real-time modulation; for this, use SVFilter. `(input=0.0, filter_type=SIGNALFLOW_FILTER_TYPE_LOW_PASS, cutoff=440, resonance=0.0, peak_gain=0.0)`
- **EQ**: Three-band EQ. `(input=0.0, low_gain=1.0, mid_gain=1.0, high_gain=1.0, low_freq=500, high_freq=5000)`
- **MoogVCF**: Moog ladder low-pass filter. `(input=0.0, cutoff=200.0, resonance=0.0)`
- **SVFilter**: State variable filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. `(input=0.0, filter_type=SIGNALFLOW_FILTER_TYPE_LOW_PASS, cutoff=440, resonance=0.0)`

## Processors: Panning

- **AzimuthPanner**: Pan input around an equally-spaced ring of num_channels speakers. pan is the pan position from -1..+1, where 0 = centre front. width is the source's width, where 1.0 spans exactly between an adjacent pair of channels. `(num_channels=2, input=0, pan=0.0, width=1.0)`
- **ChannelPanner**: Pan the input between a linear series of channels, where pan 0 = channel 0, 1 = channel 1, etc. No wrapping is applied. `(num_channels=2, input=0, pan=0.0, width=1.0)`
- **SpatialPanner**: Implements a spatial panning algorithm, applied to a given SpatialEnvironment. Currently, only DBAP is supported. `(env=nullptr, input=0.0, x=0.0, y=0.0, z=0.0, radius=1.0, algorithm="dbap")`
- **StereoBalance**: Takes a stereo input and rebalances it, where 0 is unchanged, -1 is hard left, and 1 is hard right. `(input=0, balance=0)`
- **StereoPanner**: Pans a mono input to a stereo output. Pans from -1 (hard left) to +1 (hard right), with 0 = centre. `(input=0, pan=0.0)`
- **StereoWidth**: Reduces the width of a stereo signal. When width = 1, input is unchanged. When width = 0, outputs a pair of identical channels both containing L+R. `(input=0, width=1)`

## Sequencing

- **ClockDivider**: When given a clock input (e.g., an Impulse), divides the clock by the given factor. factor must be an integer greater than or equal to 1. `(clock=0, factor=1)`
- **Counter**: Count upwards from min, driven by clock. `(clock=0, min=0, max=2147483647)`
- **Euclidean**: Euclidean rhythm as described by Toussaint, with sequence_length (n) and num_events (k), driven by clock. `(clock=0, sequence_length=0, num_events=0)`
- **FlipFlop**: Flips from 0/1 on each clock. `(clock=0)`
- **ImpulseSequence**: Each time a clock or trigger is received, outputs the next value in the sequence. At all other times, outputs zero. `(sequence=std::vector<int> ( ), clock=nullptr)`
- **Index**: Outputs the value in list corresponding to index. `(list={}, index=0)`
- **Latch**: Initially outputs 0. When a trigger is received at set, outputs 1. When a trigger is subsequently received at reset, outputs 0, until the next set. `(set=0, reset=0)`
- **Sequence**: Outputs the elements in sequence, incrementing position on each clock. `(sequence=std::vector<float> ( ), clock=nullptr)`

## Stochastic

- **Logistic**: Logistic noise. `(chaos=3.7, frequency=0.0)`
- **PinkNoise**: Pink noise, with specified low/high cutoffs. `(low_cutoff=20.0, high_cutoff=20000.0, reset=nullptr)`
- **RandomBrownian**: Outputs Brownian noise between min/max, with a mean change of delta between samples. If a clock is passed, only generates a new value on a clock tick. `(min=-1.0, max=1.0, delta=0.01, clock=nullptr, reset=nullptr)`
- **RandomChoice**: Pick a random value from the given array. If a clock is passed, only picks a new value on a clock tick. `(values=std::vector<float> ( ), clock=nullptr, reset=nullptr)`
- **RandomCoin**: Flip a coin with the given probability. If a clock is passed, only picks a new value on a clock tick. `(probability=0.5, clock=nullptr, reset=nullptr)`
- **RandomExponentialDist**: Generate an random value following the exponential distribution. If a clock is passed, only picks a new value on a clock tick. `(scale=0.0, clock=nullptr, reset=nullptr)`
- **RandomExponential**: Generate an random exponential value between min/max. If a clock is passed, only picks a new value on a clock tick. `(min=0.001, max=1.0, clock=nullptr, reset=nullptr)`
- **RandomGaussian**: Generate an random Gaussian value, with given mean and sigma. If a clock is passed, only picks a new value on a clock tick. `(mean=0.0, sigma=0.0, clock=nullptr, reset=nullptr)`
- **RandomImpulseSequence**: Generates a random sequence of 0/1 bits with the given length, and the given probability each each bit = 1. The position of the sequence is incremented on each clock signal. explore and generate are trigger inputs which cause the sequence to mutate and re-generate respectively. `(probability=0.5, length=8, clock=nullptr, explore=nullptr, generate=nullptr, reset=nullptr)`
- **RandomImpulse**: Generate random impulses at the given frequency, with either uniform or poisson distribution. `(frequency=1.0, distribution=SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM, reset=nullptr)`
- **RandomUniform**: Generates a uniformly random value between min/max. If a clock is passed, only picks a new value on a clock tick. `(min=0.0, max=1.0, clock=nullptr, reset=nullptr)`
- **StochasticNode**: StochasticNode `(reset=nullptr)`
- **WhiteNoise**: Generates whitenoise between min/max. If frequency is zero, generates at audio rate. For frequencies lower than audio rate, interpolate applies linear interpolation between values, and random_interval specifies whether new random values should be equally-spaced or randomly-spaced. `(frequency=0.0, min=-1.0, max=1.0, interpolate=true, random_interval=true, reset=nullptr)`

