#include "signalflow/python/python.h"

void init_python_nodes(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Node subclasses
     *-------------------------------------------------------------------------------*/
    py::class_<AudioIn, Node, NodeRefTemplate<AudioIn>>(m, "AudioIn")
        .def(py::init<>());

    py::class_<AudioOut_Abstract, Node, NodeRefTemplate<AudioOut_Abstract>>(m, "AudioOut_Abstract");

    py::class_<AudioOut_Dummy, AudioOut_Abstract, NodeRefTemplate<AudioOut_Dummy>>(m, "AudioOut_Dummy")
        .def(py::init<int, int>(), "num_channels"_a = 2, "buffer_size"_a = SIGNALFLOW_DEFAULT_BLOCK_SIZE);

    py::class_<AudioOut, AudioOut_Abstract, NodeRefTemplate<AudioOut>>(m, "AudioOut")
        .def(py::init<std::string, int, int>(), "device_name"_a = "", "sample_rate"_a = 0, "buffer_size"_a = 0);

    py::class_<CrossCorrelate, Node, NodeRefTemplate<CrossCorrelate>>(m, "CrossCorrelate")
        .def(py::init<NodeRef, BufferRef, int>(), "input"_a = nullptr, "buffer"_a = nullptr, "hop_size"_a = 0);

    py::class_<OnsetDetector, Node, NodeRefTemplate<OnsetDetector>>(m, "OnsetDetector")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "threshold"_a = 2.0, "min_interval"_a = 0.1);

    py::class_<BeatCutter, Node, NodeRefTemplate<BeatCutter>>(m, "BeatCutter")
        .def(py::init<BufferRef, int, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "segment_count"_a = 8, "stutter_probability"_a = 0.0, "stutter_count"_a = 1, "jump_probability"_a = 0.0, "duty_cycle"_a = 1.0, "rate"_a = 1.0, "segment_rate"_a = 1.0);

    py::class_<BufferLooper, Node, NodeRefTemplate<BufferLooper>>(m, "BufferLooper")
        .def(py::init<BufferRef, NodeRef, NodeRef, bool, bool>(), "buffer"_a = nullptr, "input"_a = 0.0, "feedback"_a = 0.0, "loop_playback"_a = false, "loop_record"_a = false);

    py::class_<BufferPlayer, Node, NodeRefTemplate<BufferPlayer>>(m, "BufferPlayer")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "rate"_a = 1.0, "loop"_a = 0, "start_time"_a = nullptr, "end_time"_a = nullptr, "clock"_a = nullptr);

    py::class_<BufferRecorder, Node, NodeRefTemplate<BufferRecorder>>(m, "BufferRecorder")
        .def(py::init<BufferRef, NodeRef, NodeRef, bool>(), "buffer"_a = nullptr, "input"_a = 0.0, "feedback"_a = 0.0, "loop"_a = false);

    py::class_<FeedbackBufferReader, Node, NodeRefTemplate<FeedbackBufferReader>>(m, "FeedbackBufferReader")
        .def(py::init<BufferRef>(), "buffer"_a = nullptr);

    py::class_<FeedbackBufferWriter, Node, NodeRefTemplate<FeedbackBufferWriter>>(m, "FeedbackBufferWriter")
        .def(py::init<BufferRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "input"_a = 0.0, "delay_time"_a = 0.1);

    py::class_<Granulator, Node, NodeRefTemplate<Granulator>>(m, "Granulator")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "clock"_a = 0, "pos"_a = 0, "duration"_a = 0.1, "pan"_a = 0.0, "rate"_a = 1.0, "max_grains"_a = 2048);

    py::class_<SegmentPlayer, Node, NodeRefTemplate<SegmentPlayer>>(m, "SegmentPlayer")
        .def(py::init<BufferRef, std::vector<float>>(), "buffer"_a = nullptr, "onsets"_a = 0);

#ifdef __APPLE__

    py::class_<MouseX, Node, NodeRefTemplate<MouseX>>(m, "MouseX")
        .def(py::init<>());

#endif

#ifdef __APPLE__

    py::class_<MouseY, Node, NodeRefTemplate<MouseY>>(m, "MouseY")
        .def(py::init<>());

#endif

#ifdef __APPLE__

    py::class_<MouseDown, Node, NodeRefTemplate<MouseDown>>(m, "MouseDown")
        .def(py::init<NodeRef>(), "button_index"_a = 0);

#endif

    py::class_<ADSREnvelope, Node, NodeRefTemplate<ADSREnvelope>>(m, "ADSREnvelope")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = 0.1, "decay"_a = 0.1, "sustain"_a = 0.5, "release"_a = 0.1, "gate"_a = 0);

    py::class_<ASREnvelope, Node, NodeRefTemplate<ASREnvelope>>(m, "ASREnvelope")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = 0.1, "sustain"_a = 0.5, "release"_a = 0.1, "curve"_a = 1.0, "clock"_a = nullptr);

    py::class_<DetectSilence, Node, NodeRefTemplate<DetectSilence>>(m, "DetectSilence")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = nullptr, "threshold"_a = 0.00001);

    py::class_<Envelope, Node, NodeRefTemplate<Envelope>>(m, "Envelope")
        .def(py::init<std::vector<NodeRef>, std::vector<NodeRef>, std::vector<NodeRef>, NodeRef, bool>(), "levels"_a = std::vector<NodeRef>(), "times"_a = std::vector<NodeRef>(), "curves"_a = std::vector<NodeRef>(), "clock"_a = nullptr, "loop"_a = false);

    py::class_<Line, Node, NodeRefTemplate<Line>>(m, "Line")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "from"_a = 0.0, "to"_a = 1.0, "time"_a = 1.0, "loop"_a = 0, "clock"_a = nullptr);

    py::class_<EnvelopeRect, Node, NodeRefTemplate<EnvelopeRect>>(m, "EnvelopeRect")
        .def(py::init<NodeRef, NodeRef>(), "sustain"_a = 1.0, "clock"_a = nullptr);

    py::class_<FFTContinuousPhaseVocoder, Node, NodeRefTemplate<FFTContinuousPhaseVocoder>>(m, "FFTContinuousPhaseVocoder")
        .def(py::init<NodeRef, float>(), "input"_a = nullptr, "rate"_a = 1.0);

#ifdef __APPLE__

    py::class_<FFTConvolve, Node, NodeRefTemplate<FFTConvolve>>(m, "FFTConvolve")
        .def(py::init<NodeRef, BufferRef>(), "input"_a = nullptr, "buffer"_a = nullptr);

#endif

    py::class_<FFT, Node, NodeRefTemplate<FFT>>(m, "FFT")
        .def(py::init<NodeRef, int, int, int, bool>(), "input"_a = 0.0, "fft_size"_a = SIGNALFLOW_DEFAULT_FFT_SIZE, "hop_size"_a = SIGNALFLOW_DEFAULT_FFT_HOP_SIZE, "window_size"_a = 0, "do_window"_a = true);

    py::class_<FFTFindPeaks, Node, NodeRefTemplate<FFTFindPeaks>>(m, "FFTFindPeaks")
        .def(py::init<NodeRef, NodeRef, NodeRef, int, bool>(), "input"_a = 0, "prominence"_a = 1, "threshold"_a = 0.000001, "count"_a = SIGNALFLOW_MAX_CHANNELS, "interpolate"_a = true);

    py::class_<IFFT, Node, NodeRefTemplate<IFFT>>(m, "IFFT")
        .def(py::init<NodeRef, bool>(), "input"_a = nullptr, "do_window"_a = false);

    py::class_<FFTLPF, Node, NodeRefTemplate<FFTLPF>>(m, "FFTLPF")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "frequency"_a = 2000);

    py::class_<FFTPhaseVocoder, Node, NodeRefTemplate<FFTPhaseVocoder>>(m, "FFTPhaseVocoder")
        .def(py::init<NodeRef>(), "input"_a = nullptr);

    py::class_<FFTTonality, Node, NodeRefTemplate<FFTTonality>>(m, "FFTTonality")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "level"_a = 0.5, "smoothing"_a = 0.9);

    py::class_<Add, Node, NodeRefTemplate<Add>>(m, "Add")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<AmplitudeToDecibels, Node, NodeRefTemplate<AmplitudeToDecibels>>(m, "AmplitudeToDecibels")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<DecibelsToAmplitude, Node, NodeRefTemplate<DecibelsToAmplitude>>(m, "DecibelsToAmplitude")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<ChannelArray, Node, NodeRefTemplate<ChannelArray>>(m, "ChannelArray")
        .def(py::init<>())
        .def(py::init<std::initializer_list<NodeRef>>(), "inputs"_a)
        .def(py::init<std::vector<NodeRef>>(), "inputs"_a)
        .def(py::init<std::vector<int>>(), "inputs"_a)
        .def(py::init<std::vector<float>>(), "inputs"_a);

    py::class_<ChannelMixer, Node, NodeRefTemplate<ChannelMixer>>(m, "ChannelMixer")
        .def(py::init<int, NodeRef, bool>(), "num_channels"_a = 1, "input"_a = 0, "amplitude_compensation"_a = true);

    py::class_<ChannelSelect, Node, NodeRefTemplate<ChannelSelect>>(m, "ChannelSelect")
        .def(py::init<NodeRef, int, int, int>(), "input"_a = nullptr, "offset"_a = 0, "maximum"_a = 0, "step"_a = 1);

    py::class_<Equal, Node, NodeRefTemplate<Equal>>(m, "Equal")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<NotEqual, Node, NodeRefTemplate<NotEqual>>(m, "NotEqual")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<GreaterThan, Node, NodeRefTemplate<GreaterThan>>(m, "GreaterThan")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<GreaterThanOrEqual, Node, NodeRefTemplate<GreaterThanOrEqual>>(m, "GreaterThanOrEqual")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<LessThan, Node, NodeRefTemplate<LessThan>>(m, "LessThan")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<LessThanOrEqual, Node, NodeRefTemplate<LessThanOrEqual>>(m, "LessThanOrEqual")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<Modulo, Node, NodeRefTemplate<Modulo>>(m, "Modulo")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<Abs, Node, NodeRefTemplate<Abs>>(m, "Abs")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<If, Node, NodeRefTemplate<If>>(m, "If")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "a"_a = 0, "value_if_true"_a = 0, "value_if_false"_a = 0);

    py::class_<Divide, Node, NodeRefTemplate<Divide>>(m, "Divide")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 1, "b"_a = 1);

    py::class_<FrequencyToMidiNote, Node, NodeRefTemplate<FrequencyToMidiNote>>(m, "FrequencyToMidiNote")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<MidiNoteToFrequency, Node, NodeRefTemplate<MidiNoteToFrequency>>(m, "MidiNoteToFrequency")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Multiply, Node, NodeRefTemplate<Multiply>>(m, "Multiply")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 1.0, "b"_a = 1.0);

    py::class_<Pow, Node, NodeRefTemplate<Pow>>(m, "Pow")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<RoundToScale, Node, NodeRefTemplate<RoundToScale>>(m, "RoundToScale")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Round, Node, NodeRefTemplate<Round>>(m, "Round")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<ScaleLinExp, Node, NodeRefTemplate<ScaleLinExp>>(m, "ScaleLinExp")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "a"_a = 0, "b"_a = 1, "c"_a = 1, "d"_a = 10);

    py::class_<ScaleLinLin, Node, NodeRefTemplate<ScaleLinLin>>(m, "ScaleLinLin")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "a"_a = 0, "b"_a = 1, "c"_a = 1, "d"_a = 10);

    py::class_<Subtract, Node, NodeRefTemplate<Subtract>>(m, "Subtract")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<Sum, Node, NodeRefTemplate<Sum>>(m, "Sum")
        .def(py::init<>())
        .def(py::init<std::initializer_list<NodeRef>>(), "inputs"_a)
        .def(py::init<std::vector<NodeRef>>(), "inputs"_a)
        .def(py::init<std::vector<int>>(), "inputs"_a)
        .def(py::init<std::vector<float>>(), "inputs"_a);

    py::class_<Sin, Node, NodeRefTemplate<Sin>>(m, "Sin")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Cos, Node, NodeRefTemplate<Cos>>(m, "Cos")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Tan, Node, NodeRefTemplate<Tan>>(m, "Tan")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Tanh, Node, NodeRefTemplate<Tanh>>(m, "Tanh")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Constant, Node, NodeRefTemplate<Constant>>(m, "Constant")
        .def(py::init<float>(), "value"_a = 0);

    py::class_<Impulse, Node, NodeRefTemplate<Impulse>>(m, "Impulse")
        .def(py::init<NodeRef>(), "frequency"_a = 1.0);

    py::class_<LFO, Node, NodeRefTemplate<LFO>>(m, "LFO")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "phase"_a = 0.0);

    py::class_<SawLFO, Node, NodeRefTemplate<SawLFO>>(m, "SawLFO")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "phase"_a = 0.0);

    py::class_<SawOscillator, Node, NodeRefTemplate<SawOscillator>>(m, "SawOscillator")
        .def(py::init<NodeRef, NodeRef>(), "frequency"_a = 440, "phase"_a = nullptr);

    py::class_<SineLFO, Node, NodeRefTemplate<SineLFO>>(m, "SineLFO")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "phase"_a = 0.0);

    py::class_<SineOscillator, Node, NodeRefTemplate<SineOscillator>>(m, "SineOscillator")
        .def(py::init<NodeRef>(), "frequency"_a = 440);

    py::class_<SquareLFO, Node, NodeRefTemplate<SquareLFO>>(m, "SquareLFO")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "width"_a = 0.5, "phase"_a = 0.0);

    py::class_<SquareOscillator, Node, NodeRefTemplate<SquareOscillator>>(m, "SquareOscillator")
        .def(py::init<NodeRef, NodeRef>(), "frequency"_a = 440, "width"_a = 0.5);

    py::class_<TriangleLFO, Node, NodeRefTemplate<TriangleLFO>>(m, "TriangleLFO")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "phase"_a = 0.0);

    py::class_<TriangleOscillator, Node, NodeRefTemplate<TriangleOscillator>>(m, "TriangleOscillator")
        .def(py::init<NodeRef>(), "frequency"_a = 440);

    py::class_<Wavetable, Node, NodeRefTemplate<Wavetable>>(m, "Wavetable")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, BufferRef>(), "buffer"_a = nullptr, "frequency"_a = 440, "phase"_a = 0, "sync"_a = 0, "phase_map"_a = nullptr);

    py::class_<Wavetable2D, Node, NodeRefTemplate<Wavetable2D>>(m, "Wavetable2D")
        .def(py::init<BufferRef2D, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "frequency"_a = 440, "crossfade"_a = 0.0, "phase"_a = 0.0, "sync"_a = 0);

    py::class_<Clip, Node, NodeRefTemplate<Clip>>(m, "Clip")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = nullptr, "min"_a = -1.0, "max"_a = 1.0);

    py::class_<AllpassDelay, Node, NodeRefTemplate<AllpassDelay>>(m, "AllpassDelay")
        .def(py::init<NodeRef, NodeRef, NodeRef, float>(), "input"_a = 0.0, "delay_time"_a = 0.1, "feedback"_a = 0.5, "max_delay_time"_a = 0.5);

    py::class_<CombDelay, Node, NodeRefTemplate<CombDelay>>(m, "CombDelay")
        .def(py::init<NodeRef, NodeRef, NodeRef, float>(), "input"_a = 0.0, "delay_time"_a = 0.1, "feedback"_a = 0.5, "max_delay_time"_a = 0.5);

    py::class_<OneTapDelay, Node, NodeRefTemplate<OneTapDelay>>(m, "OneTapDelay")
        .def(py::init<NodeRef, NodeRef, float>(), "input"_a = 0.0, "delay_time"_a = 0.1, "max_delay_time"_a = 0.5);

    py::class_<Stutter, Node, NodeRefTemplate<Stutter>>(m, "Stutter")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, float>(), "input"_a = 0.0, "stutter_time"_a = 0.1, "stutter_count"_a = 1, "clock"_a = nullptr, "max_stutter_time"_a = 1.0);

    py::class_<Resample, Node, NodeRefTemplate<Resample>>(m, "Resample")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "sample_rate"_a = 44100, "bit_rate"_a = 16);

    py::class_<SampleAndHold, Node, NodeRefTemplate<SampleAndHold>>(m, "SampleAndHold")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = nullptr, "clock"_a = nullptr);

    py::class_<Squiz, Node, NodeRefTemplate<Squiz>>(m, "Squiz")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "rate"_a = 2.0, "chunk_size"_a = 1);

    py::class_<WaveShaper, Node, NodeRefTemplate<WaveShaper>>(m, "WaveShaper")
        .def(py::init<NodeRef, BufferRef>(), "input"_a = 0.0, "buffer"_a = nullptr);

    py::class_<Compressor, Node, NodeRefTemplate<Compressor>>(m, "Compressor")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "threshold"_a = 0.1, "ratio"_a = 2, "attack_time"_a = 0.01, "release_time"_a = 0.1, "sidechain"_a = nullptr);

    py::class_<Gate, Node, NodeRefTemplate<Gate>>(m, "Gate")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0.0, "threshold"_a = 0.1);

    py::class_<Maximiser, Node, NodeRefTemplate<Maximiser>>(m, "Maximiser")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "ceiling"_a = 0.5, "attack_time"_a = 1.0, "release_time"_a = 1.0);

    py::class_<RMS, Node, NodeRefTemplate<RMS>>(m, "RMS")
        .def(py::init<NodeRef>(), "input"_a = 0.0);

    py::class_<BiquadFilter, Node, NodeRefTemplate<BiquadFilter>>(m, "BiquadFilter")
        .def(py::init<NodeRef, signalflow_filter_type_t, NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "filter_type"_a = SIGNALFLOW_FILTER_TYPE_LOW_PASS, "cutoff"_a = 440, "resonance"_a = 0.0, "peak_gain"_a = 0.0)
        .def(py::init<NodeRef, std::string, NodeRef, NodeRef, NodeRef>(), "input"_a, "filter_type"_a, "cutoff"_a = 440, "resonance"_a = 0.0, "peak_gain"_a = 0.0);

    py::class_<EQ, Node, NodeRefTemplate<EQ>>(m, "EQ")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "low_gain"_a = 1.0, "mid_gain"_a = 1.0, "high_gain"_a = 1.0, "low_freq"_a = 500, "high_freq"_a = 5000);

    py::class_<MoogVCF, Node, NodeRefTemplate<MoogVCF>>(m, "MoogVCF")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "cutoff"_a = 200.0, "resonance"_a = 0.0);

    py::class_<SVFilter, Node, NodeRefTemplate<SVFilter>>(m, "SVFilter")
        .def(py::init<NodeRef, signalflow_filter_type_t, NodeRef, NodeRef>(), "input"_a = 0.0, "filter_type"_a = SIGNALFLOW_FILTER_TYPE_LOW_PASS, "cutoff"_a = 440, "resonance"_a = 0.0)
        .def(py::init<NodeRef, std::string, NodeRef, NodeRef>(), "input"_a, "filter_type"_a, "cutoff"_a = 440, "resonance"_a = 0.0);

    py::class_<Fold, Node, NodeRefTemplate<Fold>>(m, "Fold")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = nullptr, "min"_a = -1.0, "max"_a = 1.0);

    py::class_<AzimuthPanner, Node, NodeRefTemplate<AzimuthPanner>>(m, "AzimuthPanner")
        .def(py::init<int, NodeRef, NodeRef, NodeRef>(), "num_channels"_a = 2, "input"_a = 0, "pan"_a = 0.0, "width"_a = 1.0);

    py::class_<ChannelPanner, Node, NodeRefTemplate<ChannelPanner>>(m, "ChannelPanner")
        .def(py::init<int, NodeRef, NodeRef, NodeRef>(), "num_channels"_a = 2, "input"_a = 0, "pan"_a = 0.0, "width"_a = 1.0);

    py::class_<SpatialPanner, Node, NodeRefTemplate<SpatialPanner>>(m, "SpatialPanner")
        .def(py::init<std::shared_ptr<SpatialEnvironment>, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, std::string>(), "env"_a = nullptr, "input"_a = 0.0, "x"_a = 0.0, "y"_a = 0.0, "z"_a = 0.0, "radius"_a = 1.0, "algorithm"_a = "dbap");

    py::class_<StereoBalance, Node, NodeRefTemplate<StereoBalance>>(m, "StereoBalance")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "balance"_a = 0);

    py::class_<StereoPanner, Node, NodeRefTemplate<StereoPanner>>(m, "StereoPanner")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "pan"_a = 0.0);

    py::class_<StereoWidth, Node, NodeRefTemplate<StereoWidth>>(m, "StereoWidth")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "width"_a = 1);

    py::class_<Smooth, Node, NodeRefTemplate<Smooth>>(m, "Smooth")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = nullptr, "smooth"_a = 0.99);

    py::class_<WetDry, Node, NodeRefTemplate<WetDry>>(m, "WetDry")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "dry_input"_a = nullptr, "wet_input"_a = nullptr, "wetness"_a = 0.0);

    py::class_<Wrap, Node, NodeRefTemplate<Wrap>>(m, "Wrap")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = nullptr, "min"_a = -1.0, "max"_a = 1.0);

    py::class_<ClockDivider, Node, NodeRefTemplate<ClockDivider>>(m, "ClockDivider")
        .def(py::init<NodeRef, NodeRef>(), "clock"_a = 0, "factor"_a = 1);

    py::class_<Counter, Node, NodeRefTemplate<Counter>>(m, "Counter")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "clock"_a = 0, "min"_a = 0, "max"_a = 2147483647);

    py::class_<Euclidean, Node, NodeRefTemplate<Euclidean>>(m, "Euclidean")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "clock"_a = 0, "sequence_length"_a = 0, "num_events"_a = 0);

    py::class_<FlipFlop, Node, NodeRefTemplate<FlipFlop>>(m, "FlipFlop")
        .def(py::init<NodeRef>(), "clock"_a = 0);

    py::class_<ImpulseSequence, Node, NodeRefTemplate<ImpulseSequence>>(m, "ImpulseSequence")
        .def(py::init<std::vector<int>, NodeRef>(), "sequence"_a = std::vector<int>(), "clock"_a = nullptr)
        .def(py::init<std::string, NodeRef>(), "sequence"_a, "clock"_a = nullptr);

    py::class_<Index, Node, NodeRefTemplate<Index>>(m, "Index")
        .def(py::init<std::vector<float>, NodeRef>(), "list"_a = 0, "index"_a = 0);

    py::class_<Latch, Node, NodeRefTemplate<Latch>>(m, "Latch")
        .def(py::init<NodeRef, NodeRef>(), "set"_a = 0, "reset"_a = 0);

    py::class_<Sequence, Node, NodeRefTemplate<Sequence>>(m, "Sequence")
        .def(py::init<std::vector<float>, NodeRef>(), "sequence"_a = std::vector<float>(), "clock"_a = nullptr);

    py::class_<Logistic, Node, NodeRefTemplate<Logistic>>(m, "Logistic")
        .def(py::init<NodeRef, NodeRef>(), "chaos"_a = 3.7, "frequency"_a = 0.0);

    py::class_<PinkNoise, StochasticNode, NodeRefTemplate<PinkNoise>>(m, "PinkNoise")
        .def(py::init<float, float, NodeRef>(), "low_cutoff"_a = 20.0, "high_cutoff"_a = 20000.0, "reset"_a = nullptr);

    py::class_<RandomBrownian, StochasticNode, NodeRefTemplate<RandomBrownian>>(m, "RandomBrownian")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "min"_a = -1.0, "max"_a = 1.0, "delta"_a = 0.01, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomChoice, StochasticNode, NodeRefTemplate<RandomChoice>>(m, "RandomChoice")
        .def(py::init<std::vector<float>, NodeRef, NodeRef>(), "values"_a = std::vector<float>(), "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomCoin, StochasticNode, NodeRefTemplate<RandomCoin>>(m, "RandomCoin")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "probability"_a = 0.5, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomExponentialDist, StochasticNode, NodeRefTemplate<RandomExponentialDist>>(m, "RandomExponentialDist")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "scale"_a = 0.0, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomExponential, StochasticNode, NodeRefTemplate<RandomExponential>>(m, "RandomExponential")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "min"_a = 0.001, "max"_a = 1.0, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomGaussian, StochasticNode, NodeRefTemplate<RandomGaussian>>(m, "RandomGaussian")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "mean"_a = 0.0, "sigma"_a = 0.0, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomImpulseSequence, StochasticNode, NodeRefTemplate<RandomImpulseSequence>>(m, "RandomImpulseSequence")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "probability"_a = 0.5, "length"_a = 8, "clock"_a = nullptr, "explore"_a = nullptr, "generate"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomImpulse, StochasticNode, NodeRefTemplate<RandomImpulse>>(m, "RandomImpulse")
        .def(py::init<NodeRef, signalflow_event_distribution_t, NodeRef>(), "frequency"_a = 1.0, "distribution"_a = SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM, "reset"_a = nullptr)
        .def(py::init<NodeRef, std::string, NodeRef>(), "frequency"_a, "distribution"_a, "reset"_a = nullptr);

    py::class_<RandomUniform, StochasticNode, NodeRefTemplate<RandomUniform>>(m, "RandomUniform")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "min"_a = 0.0, "max"_a = 1.0, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<WhiteNoise, StochasticNode, NodeRefTemplate<WhiteNoise>>(m, "WhiteNoise")
        .def(py::init<NodeRef, NodeRef, NodeRef, bool, bool, NodeRef>(), "frequency"_a = 0.0, "min"_a = -1.0, "max"_a = 1.0, "interpolate"_a = true, "random_interval"_a = true, "reset"_a = nullptr);
}
