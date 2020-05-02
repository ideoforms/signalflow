#include "signal/python/python.h"

void init_python_nodes(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Node subclasses
     *-------------------------------------------------------------------------------*/

    py::class_<Mixer, Node, NodeRefTemplate<Mixer>>(m, "Mixer")
        .def(py::init<NodeRef, int>(), "input"_a = NodeRef(0), "channels"_a = NodeRef(1))
        .def(py::init<float, int>(), "input"_a = NodeRef(0), "channels"_a = NodeRef(1))
        .def(py::init<std::vector<NodeRef>, int>(), "input"_a = NodeRef(0), "channels"_a = NodeRef(1))
        .def(py::init<std::vector<float>, int>(), "input"_a = NodeRef(0), "channels"_a = NodeRef(1));

    py::class_<Constant, Node, NodeRefTemplate<Constant>>(m, "Constant")
        .def(py::init<double>());

    // This is best practice for Node signatures.
    // For multichannel expansion, arrays of noderefs/floats are valid.
    py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
        .def(py::init<NodeRef>(), "frequency"_a = NodeRef(440))
        .def(py::init<float>(), "frequency"_a = NodeRef(440))
        .def(py::init<std::vector<NodeRef>>(), "frequency"_a = NodeRef(440))
        .def(py::init<std::vector<float>>(), "frequency"_a = NodeRef(440));

    py::class_<Saw, Node, NodeRefTemplate<Saw>>(m, "Saw")
        .def(py::init<NodeRef>(), "frequency"_a = NodeRef(440))
        .def(py::init<float>(), "frequency"_a = NodeRef(440))
        .def(py::init<std::vector<NodeRef>>(), "frequency"_a = NodeRef(440))
        .def(py::init<std::vector<float>>(), "frequency"_a = NodeRef(440));

    py::class_<Square, Node, NodeRefTemplate<Square>>(m, "Square")
        .def(py::init<NodeRef, NodeRef>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<NodeRef, float>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<NodeRef, std::vector<NodeRef>>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<NodeRef, std::vector<float>>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<float, NodeRef>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<float, float>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<float, std::vector<NodeRef>>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<float, std::vector<float>>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<std::vector<NodeRef>, NodeRef>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<std::vector<NodeRef>, float>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<std::vector<NodeRef>, std::vector<NodeRef>>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<std::vector<NodeRef>, std::vector<float>>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<std::vector<float>, NodeRef>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<std::vector<float>, float>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<std::vector<float>, std::vector<NodeRef>>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5))
        .def(py::init<std::vector<float>, std::vector<float>>(), "frequency"_a = NodeRef(440), "width"_a = NodeRef(0.5));

    py::class_<Tick, Node, NodeRefTemplate<Tick>>(m, "Tick")
        .def(py::init<NodeRef>(), "frequency"_a = NodeRef(440.0))
        .def(py::init<float>(), "frequency"_a = NodeRef(440.0));

    py::class_<Dust, Node, NodeRefTemplate<Dust>>(m, "Dust")
        .def(py::init<NodeRef>(), "frequency"_a = NodeRef(440.0))
        .def(py::init<float>(), "frequency"_a = NodeRef(440.0));

    py::class_<Wavetable, Node, NodeRefTemplate<Wavetable>>(m, "Wavetable")
        .def(py::init<BufferRef, NodeRef>(), "buffer"_a, "frequency"_a = NodeRef(440.0))
        .def(py::init<BufferRef, float>(), "buffer"_a, "frequency"_a = NodeRef(440.0));

    py::class_<Pan, Node, NodeRefTemplate<Pan>>(m, "Pan")
        .def(py::init<int, NodeRef, NodeRef>(), "channels"_a = 2, "input"_a = NodeRef(440.0), "pan"_a = NodeRef(0))
        .def(py::init<int, NodeRef, float>(), "channels"_a = 2, "input"_a = NodeRef(440.0), "pan"_a = NodeRef(0));

    py::class_<AudioIn, Node, NodeRefTemplate<AudioIn>>(m, "AudioIn")
        .def(py::init<>());

    py::class_<ASR, Node, NodeRefTemplate<ASR>>(m, "ASR")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = NodeRef(0.1), "sustain"_a = NodeRef(0.1), "release"_a = NodeRef(0.1), "clock"_a = NodeRef(0))
        .def(py::init<float, float, float, float>(), "attack"_a = NodeRef(0.1), "sustain"_a = NodeRef(0.1), "release"_a = NodeRef(0.1), "clock"_a = NodeRef(0))
        .def(py::init<float, float, float, NodeRef>(), "attack"_a = NodeRef(0.1), "sustain"_a = NodeRef(0.1), "release"_a = NodeRef(0.1), "clock"_a = NodeRef(0));

    py::class_<ADSR, Node, NodeRefTemplate<ADSR>>(m, "ADSR")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = NodeRef(0.1), "decay"_a = NodeRef(0.1), "sustain"_a = NodeRef(0.1), "release"_a = NodeRef(0.1), "gate"_a = NodeRef(0))
        .def(py::init<float, float, float, float, NodeRef>(), "attack"_a = NodeRef(0.1), "decay"_a = NodeRef(0.1), "sustain"_a = NodeRef(0.1), "release"_a = NodeRef(0.1), "gate"_a = NodeRef(0));

    py::class_<Sampler, Node, NodeRefTemplate<Sampler>>(m, "Sampler")
        .def(py::init<BufferRef, NodeRef, NodeRef>(), "buffer"_a, "rate"_a = NodeRef(1.0), "loop"_a = NodeRef(1))
        .def(py::init<BufferRef, float, NodeRef>(), "buffer"_a, "rate"_a = NodeRef(1.0), "loop"_a = NodeRef(1))
        .def(py::init<BufferRef, NodeRef, float>(), "buffer"_a, "rate"_a = NodeRef(1.0), "loop"_a = NodeRef(1))
        .def(py::init<BufferRef, float, float>(), "buffer"_a, "rate"_a = NodeRef(1.0), "loop"_a = NodeRef(1));

    py::class_<Granulator, Node, NodeRefTemplate<Granulator>>(m, "Granulator")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, NodeRef>(),
             "buffer"_a, "clock"_a = NodeRef(0), "pos"_a = NodeRef(0.5), "grain_length"_a = NodeRef(0.1), "rate"_a = NodeRef(1.0));

    py::class_<Delay, Node, NodeRefTemplate<Delay>>(m, "Delay")
        .def(py::init<NodeRef, NodeRef, NodeRef, float>(), "input"_a, "delaytime"_a = NodeRef(0.1), "feedback"_a = NodeRef(0.5), "maxdelaytime"_a = 1.0)
        .def(py::init<NodeRef, NodeRef, float, float>(), "input"_a, "delaytime"_a = NodeRef(0.1), "feedback"_a = NodeRef(0.5), "maxdelaytime"_a = 1.0)
        .def(py::init<NodeRef, float, NodeRef, float>(), "input"_a, "delaytime"_a = NodeRef(0.1), "feedback"_a = NodeRef(0.5), "maxdelaytime"_a = 1.0)
        .def(py::init<NodeRef, float, float, float>(), "input"_a, "delaytime"_a = NodeRef(0.1), "feedback"_a = NodeRef(0.5), "maxdelaytime"_a = 1.0);

    py::class_<Wavetable2D, Node, NodeRefTemplate<Wavetable2D>>(m, "Wavetable2D")
        .def(py::init<BufferRef2D, NodeRef, NodeRef>(), "buffer"_a, "frequency"_a = NodeRef(440.0), "crossfade"_a = NodeRef(0));
}
