#include "signal/python/python.h"

void init_python_nodes(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Node subclasses
     *-------------------------------------------------------------------------------*/

    py::class_<Mixer, Node, NodeRefTemplate<Mixer>>(m, "Mixer")
        .def(py::init<int, NodeRef>(), "channels"_a = NodeRef(1), "input"_a = NodeRef(0));

    py::class_<Constant, Node, NodeRefTemplate<Constant>>(m, "Constant")
        .def(py::init<double>());

    // This is best practice for Node signatures.
    // For multichannel expansion, arrays of noderefs/floats are valid.
    py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
        .def(py::init<NodeRefTemplate<Node>>(), "frequency"_a = NodeRef(440));

    py::class_<Saw, Node, NodeRefTemplate<Saw>>(m, "Saw")
        .def(py::init<NodeRef>(), "frequency"_a = NodeRef(440));

    py::class_<Square, Node, NodeRefTemplate<Square>>(m, "Square")
        .def(py::init<NodeRef, NodeRef>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5));

    py::class_<Impulse, Node, NodeRefTemplate<Impulse>>(m, "Impulse")
        .def(py::init<NodeRef>(), "frequency"_a = NodeRef(440.0));

    py::class_<RandomImpulse, Node, NodeRefTemplate<RandomImpulse>>(m, "RandomImpulse")
        .def(py::init<NodeRef, signal_event_distribution_t>(), "frequency"_a = NodeRef(440.0), "distribution"_a = SIGNAL_EVENT_DISTRIBUTION_UNIFORM);

    py::class_<Wavetable, Node, NodeRefTemplate<Wavetable>>(m, "Wavetable")
        .def(py::init<BufferRef, NodeRef>(), "buffer"_a, "frequency"_a = NodeRef(440.0));

    py::class_<Pan, Node, NodeRefTemplate<Pan>>(m, "Pan")
        .def(py::init<int, NodeRef, NodeRef>(), "channels"_a = 2, "input"_a = NodeRef(440.0), "pan"_a = NodeRef(0));

    py::class_<AudioIn, Node, NodeRefTemplate<AudioIn>>(m, "AudioIn")
        .def(py::init<>());

    py::class_<ASR, Node, NodeRefTemplate<ASR>>(m, "ASR")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = NodeRef(0.1), "sustain"_a = NodeRef(0.1), "release"_a = NodeRef(0.1), "clock"_a = NodeRef(0));

    py::class_<ADSR, Node, NodeRefTemplate<ADSR>>(m, "ADSR")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = NodeRef(0.1), "decay"_a = NodeRef(0.1), "sustain"_a = NodeRef(0.1), "release"_a = NodeRef(0.1), "gate"_a = NodeRef(0));

    py::class_<Sampler, Node, NodeRefTemplate<Sampler>>(m, "Sampler")
        .def(py::init<BufferRef, NodeRef, NodeRef>(), "buffer"_a, "rate"_a = NodeRef(1.0), "loop"_a = NodeRef(1));

    py::class_<Recorder, Node, NodeRefTemplate<Recorder>>(m, "Recorder")
        .def(py::init<BufferRef, NodeRef, bool>(), "buffer"_a, "input"_a = NodeRef(0.0), "loop"_a = false);

    py::class_<Granulator, Node, NodeRefTemplate<Granulator>>(m, "Granulator")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, NodeRef>(),
             "buffer"_a, "clock"_a = NodeRef(0), "pos"_a = NodeRef(0.5), "grain_length"_a = NodeRef(0.1), "rate"_a = NodeRef(1.0));

    py::class_<Delay, Node, NodeRefTemplate<Delay>>(m, "Delay")
        .def(py::init<NodeRef, NodeRef, NodeRef, float>(), "input"_a, "delaytime"_a = NodeRef(0.1), "feedback"_a = NodeRef(0.5), "maxdelaytime"_a = 1.0);

    py::class_<Wavetable2D, Node, NodeRefTemplate<Wavetable2D>>(m, "Wavetable2D")
        .def(py::init<BufferRef2D, NodeRef, NodeRef>(), "buffer"_a, "frequency"_a = NodeRef(440.0), "crossfade"_a = NodeRef(0));

    py::class_<Resample, Node, NodeRefTemplate<Resample>>(m, "Resample")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a, "sample_rate"_a = 22050, "bit_rate"_a = 8);

    py::class_<TriggerNoise, Node, NodeRefTemplate<TriggerNoise>>(m, "TriggerNoise")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "min"_a = NodeRef(0.0), "max"_a = NodeRef(0.1), "clock"_a = NodeRef(0));

    py::class_<RMS, Node, NodeRefTemplate<RMS>>(m, "RMS")
        .def(py::init<NodeRef>(), "input"_a = 0);

#ifdef __APPLE__

    py::class_<FFT, Node, NodeRefTemplate<FFT>>(m, "FFT")
        .def(py::init<NodeRef, int>(), "input"_a = NodeRef(0.0), "fft_size"_a = 1024);
    py::class_<FFTPhaseVocoder, Node, NodeRefTemplate<FFTPhaseVocoder>>(m, "FFTPhaseVocoder")
        .def(py::init<NodeRef>(), "input"_a = NodeRef(0.0));
    py::class_<IFFT, Node, NodeRefTemplate<IFFT>>(m, "IFFT")
        .def(py::init<NodeRef>(), "input"_a = NodeRef(0.0));
    py::class_<FFTTonality, Node, NodeRefTemplate<FFTTonality>>(m, "FFTTonality")
        .def(py::init<NodeRef, NodeRef, NodeRef>(), "input"_a = NodeRef(0.0), "level"_a = 0.5, "smoothing"_a = 0.9);

    py::class_<MouseX, Node, NodeRefTemplate<MouseX>>(m, "MouseX")
        .def(py::init<>());
    py::class_<MouseY, Node, NodeRefTemplate<MouseY>>(m, "MouseY")
        .def(py::init<>());

#endif
}
