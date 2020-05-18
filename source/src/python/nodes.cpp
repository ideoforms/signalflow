#include "signal/python/python.h"

void init_python_nodes(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Node subclasses
     *-------------------------------------------------------------------------------*/
    py::class_<Width, Node, NodeRefTemplate<Width>>(m, "Width")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "width"_a = 1);
    
    py::class_<Resample, Node, NodeRefTemplate<Resample>>(m, "Resample")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "sample_rate"_a = 22050, "bit_rate"_a = 8);
    
    py::class_<RMS, Node, NodeRefTemplate<RMS>>(m, "RMS")
        .def(py::init<NodeRef>(), "input"_a = 0.0);
    
    py::class_<LinearPanner, Node, NodeRefTemplate<LinearPanner>>(m, "LinearPanner")
        .def(py::init<int, NodeRef, NodeRef>(), "channels"_a = 2, "input"_a = 2, "pan"_a = 0.5);
    
    py::class_<MoogVCF, Node, NodeRefTemplate<MoogVCF>>(m, "MoogVCF")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "cutoff"_a = 200.0, "resonance"_a = 0.0);
    
    py::class_<Freeze, Node, NodeRefTemplate<Freeze>>(m, "Freeze")
        .def(py::init<NodeRef>(), "input"_a = 0);
    
    py::class_<EQ, Node, NodeRefTemplate<EQ>>(m, "EQ")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0.0, "low_gain"_a = 1.0, "mid_gain"_a = 1.0, "high_gain"_a = 1.0, "low_freq"_a = 500, "high_freq"_a = 5000);
    
    py::class_<Gate, Node, NodeRefTemplate<Gate>>(m, "Gate")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0.0, "threshold"_a = 0.1);
    
    py::class_<WaveShaper, Node, NodeRefTemplate<WaveShaper>>(m, "WaveShaper")
        .def(py::init<NodeRef, BufferRef>(), "input"_a = 0.0, "buffer"_a = nullptr);
    
    py::class_<Index, Node, NodeRefTemplate<Index>>(m, "Index")
        .def(py::init<PropertyRef, NodeRef>(), "list"_a = 0, "index"_a = 0);
    
    py::class_<Counter, Node, NodeRefTemplate<Counter>>(m, "Counter")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "clock"_a = 0, "min"_a = 0, "max"_a = 0);
    
    py::class_<LinExp, Node, NodeRefTemplate<LinExp>>(m, "LinExp")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "a"_a = 0, "b"_a = 1, "c"_a = 1, "d"_a = 10);
    
    py::class_<Scale, Node, NodeRefTemplate<Scale>>(m, "Scale")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "a"_a = 0, "b"_a = 1, "c"_a = 1, "d"_a = 10);
    
    py::class_<Divide, Node, NodeRefTemplate<Divide>>(m, "Divide")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 1, "b"_a = 1);
    
    py::class_<Multiply, Node, NodeRefTemplate<Multiply>>(m, "Multiply")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 1.0, "b"_a = 1.0);
    
    py::class_<RoundToScale, Node, NodeRefTemplate<RoundToScale>>(m, "RoundToScale")
        .def(py::init<NodeRef>(), "a"_a);
    
    py::class_<Add, Node, NodeRefTemplate<Add>>(m, "Add")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);
    
    py::class_<Subtract, Node, NodeRefTemplate<Subtract>>(m, "Subtract")
        .def(py::init<NodeRef, NodeRef>(), "a"_a = 0, "b"_a = 0);
    
    py::class_<Mixer, Node, NodeRefTemplate<Mixer>>(m, "Mixer")
        .def(py::init<int, NodeRef>(), "channels"_a = 1, "input"_a = 0);
    
    py::class_<Multiplex, Node, NodeRefTemplate<Multiplex>>(m, "Multiplex")
        .def(py::init<>())
        .def(py::init<std::initializer_list<NodeRef >>(), "inputs"_a)
        .def(py::init<std::vector<NodeRef >>(), "inputs"_a)
        .def(py::init<std::vector<int >>(), "inputs"_a)
        .def(py::init<std::vector<float >>(), "inputs"_a);
    
    py::class_<Wavetable, Node, NodeRefTemplate<Wavetable>>(m, "Wavetable")
        .def(py::init<BufferRef, NodeRef>(), "table"_a = nullptr, "frequency"_a = 440);
    
    py::class_<Wavetable2D, Node, NodeRefTemplate<Wavetable2D>>(m, "Wavetable2D")
        .def(py::init<BufferRef2D, NodeRef, NodeRef>(), "buffer"_a = nullptr, "frequency"_a = 440, "crossfade"_a = 0.0);
    
    py::class_<Constant, Node, NodeRefTemplate<Constant>>(m, "Constant")
        .def(py::init<float>(), "value"_a = 0);
    
    py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
        .def(py::init<NodeRef>(), "frequency"_a = 440);
    
    py::class_<Square, Node, NodeRefTemplate<Square>>(m, "Square")
        .def(py::init<NodeRef, NodeRef>(), "frequency"_a = 440, "width"_a = 0.5);
    
    py::class_<Saw, Node, NodeRefTemplate<Saw>>(m, "Saw")
        .def(py::init<NodeRef>(), "frequency"_a = 440);
    
    py::class_<Impulse, Node, NodeRefTemplate<Impulse>>(m, "Impulse")
        .def(py::init<NodeRef>(), "frequency"_a = 1.0);
    
    py::class_<Triangle, Node, NodeRefTemplate<Triangle>>(m, "Triangle")
        .def(py::init<NodeRef>(), "frequency"_a = 440);
    
    py::class_<Line, Node, NodeRefTemplate<Line>>(m, "Line")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "time"_a = 1.0, "from"_a = 0.0, "to"_a = 1.0);
    
    py::class_<Noise, Node, NodeRefTemplate<Noise>>(m, "Noise")
        .def(py::init<NodeRef, bool, NodeRef, NodeRef>(), "frequency"_a = 0.0, "interpolate"_a = true, "min"_a = 0.0, "max"_a = 1.0);
    
    py::class_<Logistic, Node, NodeRefTemplate<Logistic>>(m, "Logistic")
        .def(py::init<NodeRef, NodeRef>(), "chaos"_a = 3.7, "frequency"_a = 0.0);
    
    py::class_<RandomImpulse, Node, NodeRefTemplate<RandomImpulse>>(m, "RandomImpulse")
        .def(py::init<NodeRef, signal_event_distribution_t>(), "frequency"_a = 1.0, "distribution"_a = SIGNAL_EVENT_DISTRIBUTION_UNIFORM);
    
    py::class_<TriggerNoise, Node, NodeRefTemplate<TriggerNoise>>(m, "TriggerNoise")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "min"_a = 0.0, "max"_a = 1.0, "clock"_a = 0.0);
    
    py::class_<EnvelopeASR, Node, NodeRefTemplate<EnvelopeASR>>(m, "EnvelopeASR")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = 0.1, "sustain"_a = 0.5, "release"_a = 0.1, "clock"_a = nullptr);
    
    py::class_<EnvelopeADSR, Node, NodeRefTemplate<EnvelopeADSR>>(m, "EnvelopeADSR")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = 0.1, "decay"_a = 0.1, "sustain"_a = 0.5, "release"_a = 0.1, "gate"_a = 0);
    
#ifdef __APPLE__

    py::class_<FFT, Node, NodeRefTemplate<FFT>>(m, "FFT")
        .def(py::init<NodeRef, int, int>(), "input"_a = 0.0, "fft_size"_a = SIGNAL_DEFAULT_FFT_SIZE, "hop_size"_a = SIGNAL_DEFAULT_FFT_HOP_SIZE);
    
    py::class_<FFTTonality, Node, NodeRefTemplate<FFTTonality>>(m, "FFTTonality")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = 0, "level"_a = 0.5, "smoothing"_a = 0.9);
    
    py::class_<FFTNode, Node, NodeRefTemplate<FFTNode>>(m, "FFTNode")
        .def(py::init<int, int>(), "fft_size"_a, "hop_size"_a);
    
    py::class_<FFTOpNode, Node, NodeRefTemplate<FFTOpNode>>(m, "FFTOpNode")
        .def(py::init<NodeRef>(), "input"_a = nullptr);
    
    py::class_<IFFT, Node, NodeRefTemplate<IFFT>>(m, "IFFT")
        .def(py::init<NodeRef>(), "input"_a = nullptr);
    
    py::class_<FFTConvolve, Node, NodeRefTemplate<FFTConvolve>>(m, "FFTConvolve")
        .def(py::init<NodeRef, BufferRef>(), "input"_a = nullptr, "buffer"_a = nullptr);
    
    py::class_<FFTPhaseVocoder, Node, NodeRefTemplate<FFTPhaseVocoder>>(m, "FFTPhaseVocoder")
        .def(py::init<NodeRef>(), "input"_a = nullptr);
    
    py::class_<FFTLPF, Node, NodeRefTemplate<FFTLPF>>(m, "FFTLPF")
        .def(py::init<NodeRef, NodeRef>(), "input"_a = 0, "frequency"_a = 2000);
    
    py::class_<MouseX, Node, NodeRefTemplate<MouseX>>(m, "MouseX")
        .def(py::init<>());
    
    py::class_<MouseY, Node, NodeRefTemplate<MouseY>>(m, "MouseY")
        .def(py::init<>());
    
    py::class_<MouseDown, Node, NodeRefTemplate<MouseDown>>(m, "MouseDown")
        .def(py::init<NodeRef>(), "button_index"_a = 0);

#endif
    
    py::class_<BufferPlayer, Node, NodeRefTemplate<BufferPlayer>>(m, "BufferPlayer")
        .def(py::init<BufferRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "rate"_a = 1.0, "loop"_a = 0);
    
    py::class_<Granulator, Node, NodeRefTemplate<Granulator>>(m, "Granulator")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "buffer"_a = nullptr, "clock"_a = 0, "pos"_a = 0, "grain_length"_a = 0.1, "rate"_a = 1.0, "max_grains"_a = 2048);
    
    py::class_<BufferRecorder, Node, NodeRefTemplate<BufferRecorder>>(m, "BufferRecorder")
        .def(py::init<BufferRef, NodeRef, bool>(), "buffer"_a = nullptr, "input"_a = 0.0, "loop"_a = false);
    
    py::class_<AllpassDelay, Node, NodeRefTemplate<AllpassDelay>>(m, "AllpassDelay")
        .def(py::init<NodeRef, NodeRef, NodeRef, float>(), "input"_a = 0.0, "delaytime"_a = 0.1, "feedback"_a = 0.5, "maxdelaytime"_a = 0.5);
    
    py::class_<OneTapDelay, Node, NodeRefTemplate<OneTapDelay>>(m, "OneTapDelay")
        .def(py::init<NodeRef, NodeRef, float>(), "input"_a = 0.0, "delaytime"_a = 0.1, "maxdelaytime"_a = 0.5);
    
    py::class_<CombDelay, Node, NodeRefTemplate<CombDelay>>(m, "CombDelay")
        .def(py::init<NodeRef, NodeRef, NodeRef, float>(), "input"_a = 0.0, "delaytime"_a = 0.1, "feedback"_a = 0.5, "maxdelaytime"_a = 0.5);
    
    
}

