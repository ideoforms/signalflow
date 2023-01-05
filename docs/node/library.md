# Node reference library

## analysis

- **CrossCorrelate** `(input=nullptr, buffer=nullptr, hop_size=0)`
- **OnsetDetector** `(input=0.0, threshold=2.0, min_interval=0.1)`
- **VampAnalysis** `(input=0.0, plugin_id="vamp-example-plugins:spectralcentroid:linearcentroid")`

## buffer

- **BeatCutter** `(buffer=nullptr, segment_count=8, stutter_probability=0.0, stutter_count=1, jump_probability=0.0, duty_cycle=1.0, rate=1.0, segment_rate=1.0)`
- **BufferLooper** `(buffer=nullptr, input=0.0, feedback=0.0, loop_playback=false, loop_record=false)`
- **BufferPlayer** `(buffer=nullptr, rate=1.0, loop=0, start_time=nullptr, end_time=nullptr, clock=nullptr)`
- **BufferRecorder** `(buffer=nullptr, input=0.0, feedback=0.0, loop=false)`
- **FeedbackBufferReader** `(buffer=nullptr)`
- **FeedbackBufferWriter** `(buffer=nullptr, input=0.0, delay_time=0.1)`
- **GrainSegments** `(buffer=nullptr, clock=0, target=0, offsets={}, values={}, durations={})`
- **Granulator** `(buffer=nullptr, clock=0, pos=0, duration=0.1, pan=0.0, rate=1.0, max_grains=2048)`
- **SegmentPlayer** `(buffer=nullptr, onsets={})`

## control

- **MouseX** `()`
- **MouseY** `()`
- **MouseDown** `(button_index=0)`

## envelope

- **ADSREnvelope** `(attack=0.1, decay=0.1, sustain=0.5, release=0.1, gate=0)`
- **ASREnvelope** `(attack=0.1, sustain=0.5, release=0.1, curve=1.0, clock=nullptr)`
- **DetectSilence** `(input=nullptr, threshold=0.00001)`
- **Envelope** `(levels=std::vector<NodeRef> ( ), times=std::vector<NodeRef> ( ), curves=std::vector<NodeRef> ( ), clock=nullptr, loop=false)`
- **Line** `(from=0.0, to=1.0, time=1.0, loop=0, clock=nullptr)`
- **EnvelopeRect** `(sustain=1.0, clock=nullptr)`

## fft

- **FFTContinuousPhaseVocoder** `(input=nullptr, rate=1.0)`
- **FFTConvolve** `(input=nullptr, buffer=nullptr)`
- **FFT** `(input=0.0, fft_size=SIGNALFLOW_DEFAULT_FFT_SIZE, hop_size=SIGNALFLOW_DEFAULT_FFT_HOP_SIZE, window_size=0, do_window=true)`
- **FFTNode** `(fft_size=None, hop_size=None, window_size=None, do_window=None)`
- **FFTOpNode** `(input=nullptr)`
- **FFTFindPeaks** `(input=0, prominence=1, threshold=0.000001, count=SIGNALFLOW_MAX_CHANNELS, interpolate=true)`
- **IFFT** `(input=nullptr, do_window=false)`
- **FFTLPF** `(input=0, frequency=2000)`
- **FFTNoiseGate** `(input=0, threshold=0.5)`
- **FFTPhaseVocoder** `(input=nullptr)`
- **FFTTonality** `(input=0, level=0.5, smoothing=0.9)`
- **FFTZeroPhase** `(input=0)`

## operators

- **Add** `(a=0, b=0)`
- **AmplitudeToDecibels** `(a=0)`
- **DecibelsToAmplitude** `(a=0)`
- **ChannelArray** `()`
- **ChannelMixer** `(num_channels=1, input=0, amplitude_compensation=true)`
- **ChannelSelect** `(input=nullptr, offset=0, maximum=0, step=1)`
- **Equal** `(a=0, b=0)`
- **NotEqual** `(a=0, b=0)`
- **GreaterThan** `(a=0, b=0)`
- **GreaterThanOrEqual** `(a=0, b=0)`
- **LessThan** `(a=0, b=0)`
- **LessThanOrEqual** `(a=0, b=0)`
- **Modulo** `(a=0, b=0)`
- **Abs** `(a=0)`
- **If** `(a=0, value_if_true=0, value_if_false=0)`
- **Divide** `(a=1, b=1)`
- **FrequencyToMidiNote** `(a=0)`
- **MidiNoteToFrequency** `(a=0)`
- **Multiply** `(a=1.0, b=1.0)`
- **Pow** `(a=0, b=0)`
- **RoundToScale** `(a=0)`
- **Round** `(a=0)`
- **ScaleLinExp** `(input=0, a=0, b=1, c=1, d=10)`
- **ScaleLinLin** `(input=0, a=0, b=1, c=1, d=10)`
- **Subtract** `(a=0, b=0)`
- **Sum** `()`
- **Sin** `(a=0)`
- **Cos** `(a=0)`
- **Tan** `(a=0)`
- **Tanh** `(a=0)`

## oscillators

- **Constant** `(value=0)`
- **Impulse** `(frequency=1.0)`
- **LFO** `(frequency=1.0, min=0.0, max=1.0, phase=0.0)`
- **SawLFO** `(frequency=1.0, min=0.0, max=1.0, phase=0.0)`
- **SawOscillator** `(frequency=440, phase=nullptr)`
- **SineLFO** `(frequency=1.0, min=0.0, max=1.0, phase=0.0)`
- **SineOscillator** `(frequency=440)`
- **SquareLFO** `(frequency=1.0, min=0.0, max=1.0, width=0.5, phase=0.0)`
- **SquareOscillator** `(frequency=440, width=0.5)`
- **TriangleLFO** `(frequency=1.0, min=0.0, max=1.0, phase=0.0)`
- **TriangleOscillator** `(frequency=440)`
- **Wavetable** `(buffer=nullptr, frequency=440, phase=0, sync=0, phase_map=nullptr)`
- **Wavetable2D** `(buffer=nullptr, frequency=440, crossfade=0.0, phase=0.0, sync=0)`

## processors

- **Clip** `(input=nullptr, min=-1.0, max=1.0)`
- **Fold** `(input=nullptr, min=-1.0, max=1.0)`
- **Smooth** `(input=nullptr, smooth=0.99)`
- **WetDry** `(dry_input=nullptr, wet_input=nullptr, wetness=0.0)`
- **Wrap** `(input=nullptr, min=-1.0, max=1.0)`

## processors/delays

- **AllpassDelay** `(input=0.0, delay_time=0.1, feedback=0.5, max_delay_time=0.5)`
- **CombDelay** `(input=0.0, delay_time=0.1, feedback=0.5, max_delay_time=0.5)`
- **OneTapDelay** `(input=0.0, delay_time=0.1, max_delay_time=0.5)`
- **Stutter** `(input=0.0, stutter_time=0.1, stutter_count=1, clock=nullptr, max_stutter_time=1.0)`

## processors/distortion

- **Resample** `(input=0, sample_rate=44100, bit_rate=16)`
- **SampleAndHold** `(input=nullptr, clock=nullptr)`
- **Squiz** `(input=0.0, rate=2.0, chunk_size=1)`
- **WaveShaper** `(input=0.0, buffer=nullptr)`

## processors/dynamics

- **Compressor** `(input=0.0, threshold=0.1, ratio=2, attack_time=0.01, release_time=0.1, sidechain=nullptr)`
- **Gate** `(input=0.0, threshold=0.1)`
- **Maximiser** `(input=0.0, ceiling=0.5, attack_time=1.0, release_time=1.0)`
- **RMS** `(input=0.0)`

## processors/filters

- **BiquadFilter** `(input=0.0, filter_type=SIGNALFLOW_FILTER_TYPE_LOW_PASS, cutoff=440, resonance=0.0, peak_gain=0.0)`
- **EQ** `(input=0.0, low_gain=1.0, mid_gain=1.0, high_gain=1.0, low_freq=500, high_freq=5000)`
- **MoogVCF** `(input=0.0, cutoff=200.0, resonance=0.0)`
- **SVFilter** `(input=0.0, filter_type=SIGNALFLOW_FILTER_TYPE_LOW_PASS, cutoff=440, resonance=0.0)`

## processors/panning

- **AzimuthPanner** `(num_channels=2, input=0, pan=0.0, width=1.0)`
- **ChannelPanner** `(num_channels=2, input=0, pan=0.0, width=1.0)`
- **SpatialPanner** `(env=nullptr, input=0.0, x=0.0, y=0.0, z=0.0, radius=1.0, algorithm="dbap")`
- **StereoBalance** `(input=0, balance=0)`
- **StereoPanner** `(input=0, pan=0.0)`
- **StereoWidth** `(input=0, width=1)`

## sequencing

- **ClockDivider** `(clock=0, factor=1)`
- **Counter** `(clock=0, min=0, max=2147483647)`
- **Euclidean** `(clock=0, sequence_length=0, num_events=0)`
- **FlipFlop** `(clock=0)`
- **ImpulseSequence** `(sequence=std::vector<int> ( ), clock=nullptr)`
- **Index** `(list={}, index=0)`
- **Latch** `(set=0, reset=0)`
- **Sequence** `(sequence=std::vector<float> ( ), clock=nullptr)`

## stochastic

- **Logistic** `(chaos=3.7, frequency=0.0)`
- **PinkNoise** `(low_cutoff=20.0, high_cutoff=20000.0, reset=nullptr)`
- **RandomBrownian** `(min=-1.0, max=1.0, delta=0.01, clock=nullptr, reset=nullptr)`
- **RandomChoice** `(values=std::vector<float> ( ), clock=nullptr, reset=nullptr)`
- **RandomCoin** `(probability=0.5, clock=nullptr, reset=nullptr)`
- **RandomExponentialDist** `(scale=0.0, clock=nullptr, reset=nullptr)`
- **RandomExponential** `(min=0.001, max=1.0, clock=nullptr, reset=nullptr)`
- **RandomGaussian** `(mean=0.0, sigma=0.0, clock=nullptr, reset=nullptr)`
- **RandomImpulseSequence** `(probability=0.5, length=8, clock=nullptr, explore=nullptr, generate=nullptr, reset=nullptr)`
- **RandomImpulse** `(frequency=1.0, distribution=SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM, reset=nullptr)`
- **RandomUniform** `(min=0.0, max=1.0, clock=nullptr, reset=nullptr)`
- **StochasticNode** `(reset=nullptr)`
- **WhiteNoise** `(frequency=0.0, min=-1.0, max=1.0, interpolate=true, random_interval=true, reset=nullptr)`

