#include "signalflow/python/python.h"

void init_python_nodes(py::module &m)
{
    /*--------------------------------------------------------------------------------
         * Node subclasses
         *-------------------------------------------------------------------------------*/
    py::class_<AudioIn, Node, NodeRefTemplate<AudioIn>>(m, "AudioIn", "Audio input")
        .def(py::init<int>(), "num_channels"_a = 1);

    py::class_<AudioOut_Abstract, Node, NodeRefTemplate<AudioOut_Abstract>>(m, "AudioOut_Abstract", "Abstract audio output");

    py::class_<AudioOut_Dummy, Node, NodeRefTemplate<AudioOut_Dummy>>(m, "AudioOut_Dummy", "Dummy audio output for offline processing")
        .def(py::init<int, int>(), "num_channels"_a = 2, "buffer_size"_a = SIGNALFLOW_DEFAULT_BLOCK_SIZE);

    py::class_<AudioOut, Node, NodeRefTemplate<AudioOut>>(m, "AudioOut", "Audio output")
        .def(py::init<std::string, std::string, int, int>(), "backend_name"_a = "", "device_name"_a = "", "sample_rate"_a = 0, "buffer_size"_a = 0);

    py::class_<CrossCorrelate, Node, NodeRefTemplate<CrossCorrelate>>(m, "CrossCorrelate", "Outputs the cross-correlation of the input signal with the given buffer.\nIf hop_size is zero, calculates the cross-correlation every sample.\n")
        .def(py::init<NodeRef, BufferRef, int>(), "input"_a = nullptr, "buffer"_a = nullptr, "hop_size"_a = 0);

    py::class_<NearestNeighbour, Node, NodeRefTemplate<NearestNeighbour>>(m, "NearestNeighbour", "Nearest Neighbour.\n")
        .def(py::init<BufferRef, NodeRef>(), "buffer"_a = nullptr, "target"_a = 0.0);

    py::class_<OnsetDetector, Node, NodeRefTemplate<OnsetDetector>>(m, "OnsetDetector", "Simple time-domain onset detector. Outputs an impulse when an onset is detected\nin the input. Maintains short-time and long-time averages. An onset is registered\nwhen the short-time average is threshold x the long-time average.\nmin_interval is the minimum interval between onsets, in seconds.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "threshold"_a = 2.0, "min_interval"_a = 0.1);

#ifdef HAVE_VAMP

    py::class_<VampAnalysis, Node, NodeRefTemplate<VampAnalysis>>(m, "VampAnalysis", "Feature extraction using the Vamp plugin toolkit.\n")
        .def(py::init<NodeRef, std::string>(), "input"_a = 0.0, "plugin_id"_a = "vamp-example-plugins:spectralcentroid:linearcentroid")
        .def("list_plugins", &VampAnalysis::list_plugins, R"pbdoc(list[str]: List the available plugin names.)pbdoc");

#endif

    py::class_<BeatCutter, Node, NodeRefTemplate<BeatCutter>>(m, "BeatCutter", "Cuts a buffer into segment_count segments, and stutters/jumps with\nthe given probabilities.\n")
        .def(py::init<BufferRef, int, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "segment_count"_a = 8, "stutter_probability"_a = 0.0, "stutter_count"_a = 1, "jump_probability"_a = 0.0, "duty_cycle"_a = 1.0, "rate"_a = 1.0, "segment_rate"_a = 1.0);

    py::class_<BufferLooper, Node, NodeRefTemplate<BufferLooper>>(m, "BufferLooper", "Read and write from a buffer concurrently, with controllable overdub.\n")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "input"_a = 0.0, "feedback"_a = 0.0, "loop_playback"_a = 0, "loop_record"_a = 0, "start_time"_a = nullptr, "end_time"_a = nullptr, "looper_level"_a = 1.0, "playthrough_level"_a = 0.0);

    py::class_<BufferPlayer, Node, NodeRefTemplate<BufferPlayer>>(m, "BufferPlayer", "Plays the contents of the given buffer. `start_time`/`end_time` are in seconds.\nWhen a `clock` signal is received, rewinds to the `start_time`.\n\nSet `clock` to `0` to prevent the buffer from being triggered immediately.\n")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "rate"_a = 1.0, "loop"_a = 0, "start_time"_a = nullptr, "end_time"_a = nullptr, "clock"_a = nullptr);

    py::class_<BufferRecorder, Node, NodeRefTemplate<BufferRecorder>>(m, "BufferRecorder", "Records the input to a buffer. feedback controls overdub.\n")
        .def(py::init<BufferRef, NodeRef, NodeRef, bool>(), "buffer"_a = nullptr, "input"_a = 0.0, "feedback"_a = 0.0, "loop"_a = false);

    py::class_<FeedbackBufferReader, Node, NodeRefTemplate<FeedbackBufferReader>>(m, "FeedbackBufferReader", "Counterpart to FeedbackBufferWriter.\n")
        .def(py::init<BufferRef>(), "buffer"_a = nullptr);

    py::class_<FeedbackBufferWriter, Node, NodeRefTemplate<FeedbackBufferWriter>>(m, "FeedbackBufferWriter", "Counterpart to FeedbackBufferReader.\n")
        .def(py::init<BufferRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "input"_a = 0.0, "delay_time"_a = 0.1);

    py::class_<HistoryBufferWriter, Node, NodeRefTemplate<HistoryBufferWriter>>(m, "HistoryBufferWriter", "Writes a rolling history buffer of a given duration. At a given moment in time,\nthe contents of the buffer will be equal to the past N seconds of the audio\ngenerated by `input`. This is useful for (e.g.) a visual display of a rolling\nwaveform or LFO window. `downsample` can be used to downsample the input;\nfor example, with `downsample` of 10, a 1-second buffer can be used to display\n10 seconds of historical audio.\n")
        .def(py::init<BufferRef, NodeRef, int>(), "buffer"_a = nullptr, "input"_a = 0.0, "downsample"_a = 1);

    py::class_<SegmentPlayer, Node, NodeRefTemplate<SegmentPlayer>>(m, "SegmentPlayer", "Trigger segments of `buffer` at the given list of `onsets` positions, in\nseconds. `index` determines the index of the onset to play back at, which can\nalso be passed as an argument to trigger(). `rate` determines the playback rate,\nand `clock` can be used to retrigger based on the output of another Node.\nIf `continue_after_segment` is non-zero, playback will continue after the\nsubsequent onset.\n")
        .def(py::init<BufferRef, std::vector<float>, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "onsets"_a = 0, "index"_a = nullptr, "rate"_a = 1.0, "start_offset"_a = nullptr, "clock"_a = nullptr, "continue_after_segment"_a = 0);

    py::class_<SegmentedGranulator, Node, NodeRefTemplate<SegmentedGranulator>>(m, "SegmentedGranulator", "Segmented Granulator.\n")
        .def(py::init<BufferRef, std::vector<float>, std::vector<float>, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "onset_times"_a = 0, "durations"_a = 0, "index"_a = 0.0, "rate"_a = 1.0, "clock"_a = 0, "max_grains"_a = 2048);

    py::class_<Granulator, Node, NodeRefTemplate<Granulator>>(m, "Granulator", "Granulator. Generates a grain from the given buffer each time a trigger is\nreceived on the `clock` input. Each new grain uses the given `duration`,\n`amplitude`, `pan` and `rate` values presented at each input at the moment the\ngrain is created. The input buffer can be mono or stereo. If `wrap` is true,\ngrain playback can wrap around the end/start of the buffer.\n")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, bool>(), "buffer"_a = nullptr, "clock"_a = 0, "pos"_a = 0, "duration"_a = 0.1, "amplitude"_a = 1.0, "pan"_a = 0.0, "rate"_a = 1.0, "max_grains"_a = 2048, "wrap"_a = false);

#ifdef __APPLE__

    py::class_<MouseX, Node, NodeRefTemplate<MouseX>>(m, "MouseX", "Outputs the normalised cursor X position, from 0 to 1.\nCurrently only supported on macOS.\n")
        .def(py::init<>());

#endif

#ifdef __APPLE__

    py::class_<MouseY, Node, NodeRefTemplate<MouseY>>(m, "MouseY", "Outputs the normalised cursor Y position, from 0 to 1.\nCurrently only supported on macOS.\n")
        .def(py::init<>());

#endif

#ifdef __APPLE__

    py::class_<MouseDown, Node, NodeRefTemplate<MouseDown>>(m, "MouseDown", "Outputs 1 if the left mouse button is down, 0 otherwise.\nCurrently only supported on macOS.\n")
        .def(py::init<NodeRef>(), "button_index"_a = 0);

#endif

    py::class_<Accumulator, Node, NodeRefTemplate<Accumulator>>(m, "Accumulator", "Accumulator with decay.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "strike_force"_a = 0.5, "decay_coefficient"_a = 0.9999, "trigger"_a = nullptr);

    py::class_<ADSREnvelope, Node, NodeRefTemplate<ADSREnvelope>>(m, "ADSREnvelope", "Attack-decay-sustain-release envelope.\nSustain portion is held until gate is zero.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = 0.1, "decay"_a = 0.1, "sustain"_a = 0.5, "release"_a = 0.1, "gate"_a = 0);

    py::class_<ASREnvelope, Node, NodeRefTemplate<ASREnvelope>>(m, "ASREnvelope", "Attack-sustain-release envelope.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = 0.1, "sustain"_a = 0.5, "release"_a = 0.1, "curve"_a = 1.0, "clock"_a = nullptr);

    py::class_<DetectSilence, Node, NodeRefTemplate<DetectSilence>>(m, "DetectSilence", "Detects blocks of silence below the threshold value. Used as an auto-free\nnode to terminate a Patch after processing is complete.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = nullptr, "threshold"_a = 0.00001);

    py::class_<Envelope, Node, NodeRefTemplate<Envelope>>(m, "Envelope", "Generic envelope constructor, given an array of levels, times and curves.\n")
        .def(py::init<std::vector<NodeRef>, std::vector<NodeRef>, std::vector<NodeRef>, NodeRef, bool>(), "levels"_a = std::vector<NodeRef>(), "times"_a = std::vector<NodeRef>(), "curves"_a = std::vector<NodeRef>(), "clock"_a = nullptr, "loop"_a = false);

    py::class_<Line, Node, NodeRefTemplate<Line>>(m, "Line", "Line segment with the given start/end values, and duration (in seconds).\nIf loop is true, repeats indefinitely.\nRetriggers on a clock signal.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "start"_a = 0.0, "end"_a = 1.0, "time"_a = 1.0, "loop"_a = 0, "clock"_a = nullptr);

    py::class_<RectangularEnvelope, Node, NodeRefTemplate<RectangularEnvelope>>(m, "RectangularEnvelope", "Rectangular envelope with the given sustain duration.\n")
        .def(py::init<NodeRef, NodeRef>(), "sustain_duration"_a = 1.0, "clock"_a = nullptr);

    py::class_<FFTContinuousPhaseVocoder, FFTNode, NodeRefTemplate<FFTContinuousPhaseVocoder>>(m, "FFTContinuousPhaseVocoder", "Continuous phase vocoder.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef, float>(), "input"_a = nullptr, "rate"_a = 1.0);

#ifdef __APPLE__

    py::class_<FFTConvolve, FFTOpNode, NodeRefTemplate<FFTConvolve>>(m, "FFTConvolve", "Frequency-domain convolution, using overlap-add.\nUseful for convolution reverb, with the input buffer containing an impulse response.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef, BufferRef>(), "input"_a = nullptr, "buffer"_a = nullptr);

#endif

    py::class_<FFTBufferPlayer, FFTNode, NodeRefTemplate<FFTBufferPlayer>>(m, "FFTBufferPlayer", "FFTBufferPlayer.\nPlays from a buffer of audio spectra in mag/phase format.\n")
        .def(py::init<FFTBufferRef, NodeRef>(), "buffer"_a = nullptr, "rate"_a = 1.0);

    py::class_<FFTContrast, FFTOpNode, NodeRefTemplate<FFTContrast>>(m, "FFTContrast", "FFT Contrast.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "contrast"_a = 1);

    py::class_<FFTCrossFade, FFTOpNode, NodeRefTemplate<FFTCrossFade>>(m, "FFTCrossFade", "FFT FFTCrossFade.\nRequires two FFT* inputs.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "inputA"_a = 0, "inputB"_a = 0, "crossfade"_a = 0.0);

    py::class_<FFTLFO, FFTOpNode, NodeRefTemplate<FFTLFO>>(m, "FFTLFO", "FFT LFO.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "frequency"_a = 1.0, "spectral_cycles"_a = 1.0);

    py::class_<FFTMagnitudePhaseArray, FFTOpNode, NodeRefTemplate<FFTMagnitudePhaseArray>>(m, "FFTMagnitudePhaseArray", "Fixed mag/phase array.\n")
        .def(py::init<NodeRef, std::vector<float>, std::vector<float>>(), "input"_a = 0, "magnitudes"_a = 0, "phases"_a = 0)
        .def("set_magnitudes", &FFTMagnitudePhaseArray::set_magnitudes);

    py::class_<FFTRandomPhase, FFTOpNode, NodeRefTemplate<FFTRandomPhase>>(m, "FFTRandomPhase", "Randomise phase values.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "level"_a = 1.0);

    py::class_<FFTScaleMagnitudes, FFTOpNode, NodeRefTemplate<FFTScaleMagnitudes>>(m, "FFTScaleMagnitudes", "Randomise phase values.\n")
        .def(py::init<NodeRef, std::vector<float>>(), "input"_a = 0, "scale"_a = 0);

    py::class_<FFTTransform, FFTOpNode, NodeRefTemplate<FFTTransform>>(m, "FFTTransform", "Transforms the FFT magnitude spectrum in the X axis.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "flip"_a = 0, "rotate"_a = 0);

    py::class_<FFT, FFTNode, NodeRefTemplate<FFT>>(m, "FFT", "Fast Fourier Transform.\nTakes a time-domain input, and generates a frequency-domain (FFT) output.\n")
        .def(py::init<NodeRef, int, int, int, bool>(), "input"_a = 0.0, "fft_size"_a = SIGNALFLOW_DEFAULT_FFT_SIZE, "hop_size"_a = SIGNALFLOW_DEFAULT_FFT_HOP_SIZE, "window_size"_a = 0, "do_window"_a = true);

    py::class_<FFTFindPeaks, FFTOpNode, NodeRefTemplate<FFTFindPeaks>>(m, "FFTFindPeaks", "Find peaks in the FFT magnitude spectrum.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, int, bool>(), "input"_a = 0, "prominence"_a = 1, "threshold"_a = 0.000001, "count"_a = SIGNALFLOW_MAX_CHANNELS, "interpolate"_a = true);

    py::class_<IFFT, FFTOpNode, NodeRefTemplate<IFFT>>(m, "IFFT", "Inverse Fast Fourier Transform.\nRequires an FFT* input, generates a time-domain output.\n")
        .def(py::init<NodeRef, bool>(), "input"_a = nullptr, "do_window"_a = false);

    py::class_<FFTLPF, FFTOpNode, NodeRefTemplate<FFTLPF>>(m, "FFTLPF", "FFT-based brick wall low pass filter.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "frequency"_a = 2000);

    py::class_<FFTNoiseGate, FFTOpNode, NodeRefTemplate<FFTNoiseGate>>(m, "FFTNoiseGate", "FFT-based noise gate.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "threshold"_a = 0.5, "invert"_a = 0.0);

    py::class_<FFTPhaseVocoder, FFTOpNode, NodeRefTemplate<FFTPhaseVocoder>>(m, "FFTPhaseVocoder", "Phase vocoder.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef>(), "input"_a = nullptr);

    py::class_<FFTTonality, FFTOpNode, NodeRefTemplate<FFTTonality>>(m, "FFTTonality", "Tonality filter.\nRequires an FFT* input.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "level"_a = 0.5, "smoothing"_a = 0.9);

    py::class_<Add, Node, NodeRefTemplate<Add>>(m, "Add", "Add each sample of a to each sample of b.\nCan also be written as a + b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<AmplitudeToDecibels, Node, NodeRefTemplate<AmplitudeToDecibels>>(m, "AmplitudeToDecibels", "Map a linear amplitude value to decibels.\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<DecibelsToAmplitude, Node, NodeRefTemplate<DecibelsToAmplitude>>(m, "DecibelsToAmplitude", "DecibelsToAmplitude")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Bus, Node, NodeRefTemplate<Bus>>(m, "Bus", "Bus is a node with a fixed number of input channels and arbitrary number of\ninputs, used to aggregate multiple sources. It is similar to Sum, but with\na defined channel count that does not adapt to its inputs.\n")
        .def(py::init<unsigned int>(), "num_channels"_a = 1);

    py::class_<ChannelArray, Node, NodeRefTemplate<ChannelArray>>(m, "ChannelArray", "Takes an array of inputs and spreads them across multiple channels of output.\n")
        .def(py::init<>())
        .def(py::init<std::initializer_list<NodeRef>>(), "inputs"_a)
        .def(py::init<std::vector<NodeRef>>(), "inputs"_a)
        .def(py::init<std::vector<int>>(), "inputs"_a)
        .def(py::init<std::vector<float>>(), "inputs"_a);

    py::class_<ChannelCrossfade, Node, NodeRefTemplate<ChannelCrossfade>>(m, "ChannelCrossfade", "Given a multichannel input, crossfades between channels based on the given\nposition within the virtual array, producing a single-channel output.\n")
        .def(py::init<NodeRef, NodeRef, int>(), "input"_a = nullptr, "index"_a = nullptr, "num_output_channels"_a = 1);

    py::class_<ChannelMixer, Node, NodeRefTemplate<ChannelMixer>>(m, "ChannelMixer", "Downmix a multichannel input to a lower-channel output. If num_channels is\ngreater than one, spreads the input channels across the field.\nIf amplitude_compensation is enabled, scale down the amplitude based on the\nratio of input to output channels.\n")
        .def(py::init<int, NodeRef, bool>(), "num_channels"_a = 1, "input"_a = 0, "amplitude_compensation"_a = true);

    py::class_<ChannelOffset, Node, NodeRefTemplate<ChannelOffset>>(m, "ChannelOffset", "Offsets the input by a specified number of channels. With an N-channel input\nand an offset of M, the output will have M+N channels.\n")
        .def(py::init<int, NodeRef>(), "offset"_a = 0, "input"_a = nullptr);

    py::class_<ChannelSelect, Node, NodeRefTemplate<ChannelSelect>>(m, "ChannelSelect", "Select a subset of channels from a multichannel input, starting at offset,\nup to a maximum of maximum, with the given step.\n")
        .def(py::init<NodeRef, int, int, int>(), "input"_a = nullptr, "offset"_a = 0, "maximum"_a = 0, "step"_a = 1);

    py::class_<Equal, Node, NodeRefTemplate<Equal>>(m, "Equal", "Compares the output of a to the output of b. Outputs 1 when equal, 0 otherwise.\nCan also be written as a == b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<NotEqual, Node, NodeRefTemplate<NotEqual>>(m, "NotEqual", "Compares the output of a to the output of b. Outputs 0 when equal, 1 otherwise.\nCan also be written as a != b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<GreaterThan, Node, NodeRefTemplate<GreaterThan>>(m, "GreaterThan", "Compares the output of a to the output of b. Outputs 1 when a > b, 0 otherwise.\nCan also be written as a > b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<GreaterThanOrEqual, Node, NodeRefTemplate<GreaterThanOrEqual>>(m, "GreaterThanOrEqual", "Compares the output of a to the output of b. Outputs 1 when a >= b, 0 otherwise.\nCan also be written as a >= b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<LessThan, Node, NodeRefTemplate<LessThan>>(m, "LessThan", "Compares the output of a to the output of b. Outputs 1 when a < b, 0 otherwise.\nCan also be written as a < b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<LessThanOrEqual, Node, NodeRefTemplate<LessThanOrEqual>>(m, "LessThanOrEqual", "Compares the output of a to the output of b. Outputs 1 when a <= b, 0 otherwise.\nCan also be written as a <= b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<Modulo, Node, NodeRefTemplate<Modulo>>(m, "Modulo", "Outputs the value of a modulo b, per sample. Supports fractional values.\nCan also be written as a % b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<Abs, Node, NodeRefTemplate<Abs>>(m, "Abs", "Outputs the absolute value of a, per sample.\nCan also be written as abs(a)\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<If, Node, NodeRefTemplate<If>>(m, "If", "Outputs value_if_true for each non-zero value of a, value_if_false for all\nother values.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "a"_a = 0, "value_if_true"_a = 0, "value_if_false"_a = 0);

    py::class_<Divide, Node, NodeRefTemplate<Divide>>(m, "Divide", "Divide each sample of a by each sample of b.\nCan also be written as a / b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 1, "b"_a = 1);

    py::class_<FrequencyToMidiNote, Node, NodeRefTemplate<FrequencyToMidiNote>>(m, "FrequencyToMidiNote", "Map a frequency to a MIDI note (where 440Hz = A4 = 69), with floating-point\noutput.\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<MidiNoteToFrequency, Node, NodeRefTemplate<MidiNoteToFrequency>>(m, "MidiNoteToFrequency", "Map a MIDI note to a frequency (where 440Hz = A4 = 69), supporting floating-point\ninput.\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Multiply, Node, NodeRefTemplate<Multiply>>(m, "Multiply", "Multiply each sample of a by each sample of b.\nCan also be written as a * b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 1.0, "b"_a = 1.0);

    py::class_<Pow, Node, NodeRefTemplate<Pow>>(m, "Pow", "Outputs a to the power of b, per sample.\nCan also be written as a ** b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<RoundToScale, Node, NodeRefTemplate<RoundToScale>>(m, "RoundToScale", "Given a frequency input, generates a frequency output that is rounded to the nearest MIDI note.\n(TODO: Not very well named)\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Round, Node, NodeRefTemplate<Round>>(m, "Round", "Round the input to the nearest integer value.\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<ScaleLinExp, Node, NodeRefTemplate<ScaleLinExp>>(m, "ScaleLinExp", "Scales the input from a linear range (between a and b)\nto an exponential range (between c and d).\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "a"_a = 0, "b"_a = 1, "c"_a = 1, "d"_a = 10);

    py::class_<ScaleLinLin, Node, NodeRefTemplate<ScaleLinLin>>(m, "ScaleLinLin", "Scales the input from a linear range (between a and b)\nto a linear range (between c and d).\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "a"_a = 0, "b"_a = 1, "c"_a = 1, "d"_a = 10);

    py::class_<SelectInput, Node, NodeRefTemplate<SelectInput>>(m, "SelectInput", "Pass through the output of one or more `inputs`, based on the integer input index\nspecified in `index`. Unlike `ChannelSelect`, inputs may be multichannel,\nand `index` can be modulated in real time.\n")
        .def(py::init<NodeRef>(), "index"_a = 0)
        .def(py::init<std::initializer_list<NodeRef>, NodeRef>(), "inputs"_a, "index"_a = 0)
        .def(py::init<std::vector<NodeRef>, NodeRef>(), "inputs"_a, "index"_a = 0)
        .def(py::init<std::vector<int>, NodeRef>(), "inputs"_a, "index"_a = 0)
        .def(py::init<std::vector<float>, NodeRef>(), "inputs"_a, "index"_a = 0);

    py::class_<Subtract, Node, NodeRefTemplate<Subtract>>(m, "Subtract", "Subtract each sample of b from each sample of a.\nCan also be written as a - b\n")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);

    py::class_<Sum, Node, NodeRefTemplate<Sum>>(m, "Sum", "Sums the output of all of the input nodes, by sample.\n")
        .def(py::init<>())
        .def(py::init<std::initializer_list<NodeRef>>(), "inputs"_a)
        .def(py::init<std::vector<NodeRef>>(), "inputs"_a)
        .def(py::init<std::vector<int>>(), "inputs"_a)
        .def(py::init<std::vector<float>>(), "inputs"_a);

    py::class_<TimeShift, Node, NodeRefTemplate<TimeShift>>(m, "TimeShift", "TimeShift\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Sin, Node, NodeRefTemplate<Sin>>(m, "Sin", "Outputs sin(a), per sample.\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Cos, Node, NodeRefTemplate<Cos>>(m, "Cos", "Outputs cos(a), per sample.\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Tan, Node, NodeRefTemplate<Tan>>(m, "Tan", "Outputs tan(a), per sample.\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Tanh, Node, NodeRefTemplate<Tanh>>(m, "Tanh", "Outputs tanh(a), per sample.\nCan be used as a soft clipper.\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<Constant, Node, NodeRefTemplate<Constant>>(m, "Constant", "Produces a constant value.\n")
        .def(py::init<float>(), "value"_a = 0);

    py::class_<Impulse, Node, NodeRefTemplate<Impulse>>(m, "Impulse", "Produces a value of 1 at the given `frequency`, with output of 0 at all other times.\nIf frequency is 0, produces a single impulse.\n")
        .def(py::init<NodeRef>(), "frequency"_a = 1.0);

    py::class_<LFO, Node, NodeRefTemplate<LFO>>(m, "LFO", "LFO")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "phase"_a = 0.0);

    py::class_<SawLFO, Node, NodeRefTemplate<SawLFO>>(m, "SawLFO", "Produces a sawtooth LFO at the given `frequency` and `phase` offset, with output ranging from `min` to `max`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "phase"_a = 0.0);

    py::class_<SawOscillator, Node, NodeRefTemplate<SawOscillator>>(m, "SawOscillator", "Produces a (non-band-limited) sawtooth wave, with the given `frequency` and\n`phase` offset. When a `reset` or trigger is received, resets the phase to zero.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "frequency"_a = 440, "phase_offset"_a = nullptr, "reset"_a = nullptr);

    py::class_<SineLFO, Node, NodeRefTemplate<SineLFO>>(m, "SineLFO", "Produces a sinusoidal LFO at the given `frequency` and `phase` offset,\nwith output ranging from `min` to `max`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "phase"_a = 0.0);

    py::class_<SineOscillator, Node, NodeRefTemplate<SineOscillator>>(m, "SineOscillator", "Produces a sine wave at the given `frequency`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "frequency"_a = 440, "phase_offset"_a = nullptr, "reset"_a = nullptr);

    py::class_<SquareLFO, Node, NodeRefTemplate<SquareLFO>>(m, "SquareLFO", "Produces a pulse wave LFO with the given `frequency` and pulse `width`, \nranging from `min` to `max`, where `width` of `0.5` is a square wave and\nother values produce a rectangular wave.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "width"_a = 0.5, "phase"_a = 0.0);

    py::class_<SquareOscillator, Node, NodeRefTemplate<SquareOscillator>>(m, "SquareOscillator", "Produces a pulse wave with the given `frequency` and pulse `width`, \nwhere `width` of `0.5` is a square wave and other values produce a\nrectangular wave.\n")
        .def(py::init<NodeRef, NodeRef>(), "frequency"_a = 440, "width"_a = 0.5);

    py::class_<TriangleLFO, Node, NodeRefTemplate<TriangleLFO>>(m, "TriangleLFO", "Produces a triangle LFO with the given `frequency` and `phase` offset,\nranging from `min` to `max`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "frequency"_a = 1.0, "min"_a = 0.0, "max"_a = 1.0, "phase"_a = 0.0);

    py::class_<TriangleOscillator, Node, NodeRefTemplate<TriangleOscillator>>(m, "TriangleOscillator", "Produces a triangle wave with the given `frequency`.\n")
        .def(py::init<NodeRef>(), "frequency"_a = 440);

    py::class_<Wavetable, Node, NodeRefTemplate<Wavetable>>(m, "Wavetable", "Plays the wavetable stored in buffer at the given `frequency` and `phase` offset.\n`sync` can be used to provide a hard sync input, which resets the wavetable's\nphase at each zero-crossing.\n")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, BufferRef>(), "buffer"_a = nullptr, "frequency"_a = 440, "phase_offset"_a = 0, "sync"_a = 0, "phase_map"_a = nullptr);

    py::class_<Wavetable2D, Node, NodeRefTemplate<Wavetable2D>>(m, "Wavetable2D", "Wavetable2D")
        .def(py::init<BufferRef2D, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "frequency"_a = 440, "crossfade"_a = 0.0, "phase_offset"_a = 0.0, "sync"_a = 0);

    py::class_<Maraca, StochasticNode, NodeRefTemplate<Maraca>>(m, "Maraca", "Physically-inspired model of a maraca.\n\nParameters:\n- `num_beans`: The number of simulated beans in the maraca (1-1024)\n- `shake_decay`: Decay constant for the energy injected per shake\n- `grain_decay`: Decay constant for the energy created per bean collision\n- `shake_duration`: Duration of each shake action, milliseconds\n- `shell_frequency`: Resonant frequency of the maraca's shell, hertz\n- `shell_resonance`: Resonanc of the maraca's shell (0-1)\n- `clock`: If specified, triggers shake actions\n- `energy`: If specified, injects energy into the maraca\n\nFrom Cook (1997), \"Physically Informed Sonic Modeling (PhISM): Synthesis of\nPercussive Sounds\", Computer Music Journal.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "num_beans"_a = 64, "shake_decay"_a = 0.99, "grain_decay"_a = 0.99, "shake_duration"_a = 0.02, "shell_frequency"_a = 12000, "shell_resonance"_a = 0.9, "clock"_a = nullptr, "energy"_a = nullptr);

    py::class_<Clip, Node, NodeRefTemplate<Clip>>(m, "Clip", "Clip the input to `min`/`max`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = nullptr, "min"_a = -1.0, "max"_a = 1.0);

    py::class_<Fold, Node, NodeRefTemplate<Fold>>(m, "Fold", "Fold the input beyond `min`/`max`, reflecting the excess back.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = nullptr, "min"_a = -1.0, "max"_a = 1.0);

    py::class_<Smooth, Node, NodeRefTemplate<Smooth>>(m, "Smooth", "Smooth the input with a given smoothing coefficient.\nWhen `smooth` = 0, applies no smoothing.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = nullptr, "smooth"_a = 0.99);

    py::class_<WetDry, Node, NodeRefTemplate<WetDry>>(m, "WetDry", "Takes `wet` and `dry` inputs, and outputs a mix determined by `wetness`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "dry_input"_a = nullptr, "wet_input"_a = nullptr, "wetness"_a = 0.0);

    py::class_<Wrap, Node, NodeRefTemplate<Wrap>>(m, "Wrap", "Wrap the input beyond `min`/`max`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = nullptr, "min"_a = -1.0, "max"_a = 1.0);

    py::class_<AllpassDelay, Node, NodeRefTemplate<AllpassDelay>>(m, "AllpassDelay", "All-pass delay, with `feedback` between 0 and 1.\n`delay_time` must be less than or equal to `max_delay_time`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, float>(), "input"_a = 0.0, "delay_time"_a = 0.1, "feedback"_a = 0.5, "max_delay_time"_a = 0.5);

    py::class_<CombDelay, Node, NodeRefTemplate<CombDelay>>(m, "CombDelay", "Comb delay, with `feedback` between 0 and 1.\n`delay_time` must be less than or equal to `max_delay_time`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, float>(), "input"_a = 0.0, "delay_time"_a = 0.1, "feedback"_a = 0.5, "max_delay_time"_a = 0.5);

    py::class_<OneTapDelay, Node, NodeRefTemplate<OneTapDelay>>(m, "OneTapDelay", "Single-tap delay line.\n`delay_time` must be less than or equal to `max_delay_time`.\n")
        .def(py::init<NodeRef, NodeRef, float>(), "input"_a = 0.0, "delay_time"_a = 0.1, "max_delay_time"_a = 0.5);

    py::class_<Stutter, Node, NodeRefTemplate<Stutter>>(m, "Stutter", "Stutters the input whenever a trigger is received on `clock`.\nGenerates `stutter_count` repeats, with duration of `stutter_time`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, float>(), "input"_a = 0.0, "stutter_time"_a = 0.1, "stutter_count"_a = 1, "stutter_probability"_a = 1.0, "stutter_advance_time"_a = 0.0, "clock"_a = nullptr, "max_stutter_time"_a = 1.0);

    py::class_<Resample, Node, NodeRefTemplate<Resample>>(m, "Resample", "Resampler and bit crusher. `sample_rate` is in Hz, `bit_rate` is an integer\nbetween 0 and 16.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "sample_rate"_a = 44100, "bit_rate"_a = 16);

    py::class_<SampleAndHold, Node, NodeRefTemplate<SampleAndHold>>(m, "SampleAndHold", "Samples and holds the input each time a trigger is received on `clock`.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = nullptr, "clock"_a = nullptr);

    py::class_<Squiz, Node, NodeRefTemplate<Squiz>>(m, "Squiz", "Implementation of Dan Stowell's Squiz algorithm, a kind of downsampler.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "rate"_a = 2.0, "chunk_size"_a = 1);

    py::class_<WaveShaper, Node, NodeRefTemplate<WaveShaper>>(m, "WaveShaper", "Applies wave-shaping as described in the WaveShaperBuffer `buffer`.\n")
        .def(py::init<NodeRef, BufferRef>(), "input"_a = 0.0, "buffer"_a = nullptr);

    py::class_<Compressor, Node, NodeRefTemplate<Compressor>>(m, "Compressor", "Dynamic range compression, with optional `sidechain` input.\nWhen the input amplitude is above `threshold`, compresses the amplitude with\nthe given `ratio`, following the given `attack_time` and `release_time`\nin seconds.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "threshold"_a = 0.1, "ratio"_a = 2, "attack_time"_a = 0.01, "release_time"_a = 0.1, "sidechain"_a = nullptr);

    py::class_<Gate, Node, NodeRefTemplate<Gate>>(m, "Gate", "Outputs the input value when it is above the given `threshold`, otherwise zero.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0.0, "threshold"_a = 0.1);

    py::class_<Maximiser, Node, NodeRefTemplate<Maximiser>>(m, "Maximiser", "Gain maximiser.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "ceiling"_a = 0.5, "attack_time"_a = 1.0, "release_time"_a = 1.0);

    py::class_<RMS, Node, NodeRefTemplate<RMS>>(m, "RMS", "Outputs the root-mean-squared value of the input, in buffers equal to the\ngraph's current buffer size.\n")
        .def(py::init<NodeRef>(), "input"_a = 0.0);

    py::class_<BiquadFilter, Node, NodeRefTemplate<BiquadFilter>>(m, "BiquadFilter", "Biquad filter.\nfilter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak',\n'low_shelf', 'high_shelf'.\nNot recommended for real-time modulation; for this, use SVFilter.\n")
        .def(py::init<NodeRef, signalflow_filter_type_t, NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "filter_type"_a = SIGNALFLOW_FILTER_TYPE_LOW_PASS, "cutoff"_a = 440, "resonance"_a = 0.0, "peak_gain"_a = 0.0)
        .def(py::init<NodeRef, std::string, NodeRef, NodeRef, NodeRef>(), "input"_a, "filter_type"_a, "cutoff"_a = 440, "resonance"_a = 0.0, "peak_gain"_a = 0.0);

    py::class_<DCFilter, Node, NodeRefTemplate<DCFilter>>(m, "DCFilter", "Remove low-frequency and DC content from a signal.\n")
        .def(py::init<NodeRef>(), "input"_a = 0.0);

    py::class_<EQ, Node, NodeRefTemplate<EQ>>(m, "EQ", "Three-band EQ.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "low_gain"_a = 1.0, "mid_gain"_a = 1.0, "high_gain"_a = 1.0, "low_freq"_a = 500, "high_freq"_a = 5000);

    py::class_<MoogVCF, Node, NodeRefTemplate<MoogVCF>>(m, "MoogVCF", "Simulation of the Moog ladder low-pass filter. `cutoff` sets the cutoff\nfrequency; `resonance` should typically be between 0..1.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "cutoff"_a = 200.0, "resonance"_a = 0.0);

    py::class_<SVFilter, Node, NodeRefTemplate<SVFilter>>(m, "SVFilter", "State variable filter.\n`filter_type` can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak',\n'low_shelf', 'high_shelf'. `resonance` should be between `[0..1]`.\n")
        .def(py::init<NodeRef, signalflow_filter_type_t, NodeRef, NodeRef>(), "input"_a = 0.0, "filter_type"_a = SIGNALFLOW_FILTER_TYPE_LOW_PASS, "cutoff"_a = 440, "resonance"_a = 0.0)
        .def(py::init<NodeRef, std::string, NodeRef, NodeRef>(), "input"_a, "filter_type"_a, "cutoff"_a = 440, "resonance"_a = 0.0);

    py::class_<AzimuthPanner, Node, NodeRefTemplate<AzimuthPanner>>(m, "AzimuthPanner", "Pan input around an equally-spaced ring of `num_channels` speakers.\n`pan` is the pan position from -1..+1, where 0 = centre front.\n`width` is the source's width, where 1.0 spans exactly between an adjacent pair of channels.\n")
        .def(py::init<int, NodeRef, NodeRef, NodeRef>(), "num_channels"_a = 2, "input"_a = 0, "pan"_a = 0.0, "width"_a = 1.0);

    py::class_<ChannelPanner, Node, NodeRefTemplate<ChannelPanner>>(m, "ChannelPanner", "Pan the input between a linear series of channels, where `pan` 0 = channel 0,\n1 = channel 1, etc. No wrapping is applied.\n")
        .def(py::init<int, NodeRef, NodeRef, NodeRef>(), "num_channels"_a = 2, "input"_a = 0, "pan"_a = 0.0, "width"_a = 1.0);

    py::class_<SpatialPanner, Node, NodeRefTemplate<SpatialPanner>>(m, "SpatialPanner", "Implements a spatial panning algorithm, applied to a given SpatialEnvironment.\nCurrently, only DBAP is supported.\n")
        .def(py::init<std::shared_ptr<SpatialEnvironment>, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, std::string>(), "env"_a = nullptr, "input"_a = 0.0, "x"_a = 0.0, "y"_a = 0.0, "z"_a = 0.0, "radius"_a = 1.0, "use_delays"_a = 1.0, "algorithm"_a = "dbap");

    py::class_<StereoBalance, Node, NodeRefTemplate<StereoBalance>>(m, "StereoBalance", "Takes a stereo input and rebalances it, where `balance` of `0` is unchanged,\n`-1` is hard left, and `1` is hard right.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "balance"_a = 0);

    py::class_<StereoPanner, Node, NodeRefTemplate<StereoPanner>>(m, "StereoPanner", "Pans a mono input to a stereo output. `pan` should be between -1 (hard left) to\n+1 (hard right), with 0 = centre.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "pan"_a = 0.0);

    py::class_<StereoWidth, Node, NodeRefTemplate<StereoWidth>>(m, "StereoWidth", "Reduces the width of a stereo signal.\nWhen `width` = 1, input is unchanged.\nWhen `width` = 0, outputs a pair of identical channels both containing L+R.\n")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "width"_a = 1);

    py::class_<ClockDivider, Node, NodeRefTemplate<ClockDivider>>(m, "ClockDivider", "When given a `clock` input (e.g., an Impulse), divides the clock by the given\n`factor`. factor must be an integer greater than or equal to 1.\n")
        .def(py::init<NodeRef, NodeRef>(), "clock"_a = 0, "factor"_a = 1);

    py::class_<Counter, Node, NodeRefTemplate<Counter>>(m, "Counter", "Count upwards from `min` to `max`, driven by `clock`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "clock"_a = 0, "min"_a = 0, "max"_a = 2147483647);

    py::class_<Euclidean, Node, NodeRefTemplate<Euclidean>>(m, "Euclidean", "Euclidean rhythm as described by Toussaint, with `sequence_length` (n)\nand `num_events` (k), driven by `clock`.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "clock"_a = 0, "sequence_length"_a = 0, "num_events"_a = 0);

    py::class_<FlipFlop, Node, NodeRefTemplate<FlipFlop>>(m, "FlipFlop", "Flips from 0/1 on each `clock`.\n")
        .def(py::init<NodeRef>(), "clock"_a = 0);

    py::class_<ImpulseSequence, Node, NodeRefTemplate<ImpulseSequence>>(m, "ImpulseSequence", "Each time a `clock` or trigger is received, outputs the next value in\n`sequence`. At all other times, outputs zero.\n")
        .def(py::init<std::vector<int>, NodeRef>(), "sequence"_a = std::vector<int>(), "clock"_a = nullptr)
        .def(py::init<std::string, NodeRef>(), "sequence"_a, "clock"_a = nullptr);

    py::class_<Index, Node, NodeRefTemplate<Index>>(m, "Index", "Outputs the value in `list` corresponding to `index`.\n")
        .def(py::init<std::vector<float>, NodeRef>(), "list"_a = 0, "index"_a = 0);

    py::class_<Latch, Node, NodeRefTemplate<Latch>>(m, "Latch", "Initially outputs 0.\nWhen a trigger is received at `set`, outputs 1.\nWhen a trigger is subsequently received at `reset`, outputs 0, until the next\n`set`.\n")
        .def(py::init<NodeRef, NodeRef>(), "set"_a = 0, "reset"_a = 0);

    py::class_<Sequence, Node, NodeRefTemplate<Sequence>>(m, "Sequence", "Outputs the elements in `sequence`, incrementing position on each `clock`.\n")
        .def(py::init<std::vector<float>, NodeRef>(), "sequence"_a = std::vector<float>(), "clock"_a = nullptr);

    py::class_<TriggerMult, Node, NodeRefTemplate<TriggerMult>>(m, "TriggerMult", "Distribute any triggers to all output nodes.\n")
        .def(py::init<NodeRef>(), "a"_a = 0);

    py::class_<TriggerRoundRobin, Node, NodeRefTemplate<TriggerRoundRobin>>(m, "TriggerRoundRobin", "Relay trigger() events to a single node from the list of connected outputs,\nwith `direction` determining the direction: 1 (or above) = move forwards by N,\n-1 = move backwards by N, 0 = stationary.\n")
        .def(py::init<NodeRef>(), "direction"_a = 1);

    py::class_<Logistic, Node, NodeRefTemplate<Logistic>>(m, "Logistic", "Logistic noise.\n")
        .def(py::init<NodeRef, NodeRef>(), "chaos"_a = 3.7, "frequency"_a = 0.0);

    py::class_<PinkNoise, StochasticNode, NodeRefTemplate<PinkNoise>>(m, "PinkNoise", "Pink noise, with specified low/high cutoffs.\n")
        .def(py::init<float, float, NodeRef>(), "low_cutoff"_a = 20.0, "high_cutoff"_a = 20000.0, "reset"_a = nullptr);

    py::class_<RandomBrownian, StochasticNode, NodeRefTemplate<RandomBrownian>>(m, "RandomBrownian", "Outputs Brownian noise between min/max, with a mean change of delta between samples.\nIf a clock is passed, only generates a new value on a clock tick.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "min"_a = -1.0, "max"_a = 1.0, "delta"_a = 0.01, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomChoice, StochasticNode, NodeRefTemplate<RandomChoice>>(m, "RandomChoice", "Pick a random value from the given array.\nIf a clock is passed, only picks a new value on a clock tick.\n")
        .def(py::init<std::vector<float>, NodeRef, NodeRef>(), "values"_a = std::vector<float>(), "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomCoin, StochasticNode, NodeRefTemplate<RandomCoin>>(m, "RandomCoin", "Flip a coin with the given probability.\nIf a clock is passed, only picks a new value on a clock tick.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "probability"_a = 0.5, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomExponentialDist, StochasticNode, NodeRefTemplate<RandomExponentialDist>>(m, "RandomExponentialDist", "Generate an random value following the exponential distribution.\nIf a clock is passed, only picks a new value on a clock tick.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "scale"_a = 0.0, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomExponential, StochasticNode, NodeRefTemplate<RandomExponential>>(m, "RandomExponential", "Generate an random exponential value between min/max.\nIf a clock is passed, only picks a new value on a clock tick.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "min"_a = 0.001, "max"_a = 1.0, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomGaussian, StochasticNode, NodeRefTemplate<RandomGaussian>>(m, "RandomGaussian", "Generate an random Gaussian value, with given mean and sigma.\nIf a clock is passed, only picks a new value on a clock tick.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "mean"_a = 0.0, "sigma"_a = 0.0, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomImpulseSequence, StochasticNode, NodeRefTemplate<RandomImpulseSequence>>(m, "RandomImpulseSequence", "Generates a random sequence of 0/1 bits with the given length, and the given\nprobability each each bit = 1. The position of the sequence is incremented\non each clock signal. explore and generate are trigger inputs which cause\nthe sequence to mutate and re-generate respectively.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "probability"_a = 0.5, "length"_a = 8, "clock"_a = nullptr, "explore"_a = nullptr, "generate"_a = nullptr, "reset"_a = nullptr);

    py::class_<RandomImpulse, StochasticNode, NodeRefTemplate<RandomImpulse>>(m, "RandomImpulse", "Generate random impulses at the given frequency, with either uniform\nor poisson distribution.\n")
        .def(py::init<NodeRef, signalflow_event_distribution_t, NodeRef>(), "frequency"_a = 1.0, "distribution"_a = SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM, "reset"_a = nullptr)
        .def(py::init<NodeRef, std::string, NodeRef>(), "frequency"_a, "distribution"_a, "reset"_a = nullptr);

    py::class_<RandomUniform, StochasticNode, NodeRefTemplate<RandomUniform>>(m, "RandomUniform", "Generates a uniformly random value between min/max.\nIf a clock is passed, only picks a new value on a clock tick.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "min"_a = 0.0, "max"_a = 1.0, "clock"_a = nullptr, "reset"_a = nullptr);

    py::class_<WhiteNoise, StochasticNode, NodeRefTemplate<WhiteNoise>>(m, "WhiteNoise", "Generates whitenoise between min/max. If frequency is zero, generates at\naudio rate. For frequencies lower than audio rate, interpolate applies linear\ninterpolation between values, and random_interval specifies whether new\nrandom values should be equally-spaced or randomly-spaced.\n")
        .def(py::init<NodeRef, NodeRef, NodeRef, bool, bool, NodeRef>(), "frequency"_a = 0.0, "min"_a = -1.0, "max"_a = 1.0, "interpolate"_a = true, "random_interval"_a = true, "reset"_a = nullptr);
}
