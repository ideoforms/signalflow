// clang-format off

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "signal/signal.h"

using namespace libsignal;

namespace py = pybind11;

/*------------------------------------------------------------------------------
 * Exposes the _a literal which is equivalent to py::arg
 * https://pybind11.readthedocs.io/en/stable/basics.html
 *----------------------------------------------------------------------------*/
using namespace pybind11::literals;

PYBIND11_DECLARE_HOLDER_TYPE(T, NodeRefTemplate<T>);
PYBIND11_DECLARE_HOLDER_TYPE(T, BufferRefTemplate<T>);

PYBIND11_MODULE(libsignal, m)
{
    py::class_<Node, NodeRefTemplate<Node>>(m, "Node")
        .def("__add__", [](NodeRef a, NodeRef b)
            { return NodeRef(new Add(a, b)); })
        .def("__add__", [](NodeRef a, float b)
            { return NodeRef(new Add(a, NodeRef(new Constant(b)))); })
        .def("__radd__", [](NodeRefTemplate<Node>a, float b)
            { return NodeRef(new Add(a, NodeRef(new Constant(b)))); })
        .def("__sub__", [](NodeRef a, NodeRef b)
            { return NodeRef(new Subtract(a, b)); })
        .def("__sub__", [](NodeRef a, float b)
            { return NodeRef(new Subtract(a, NodeRef(new Constant(b)))); })
        .def("__rsub__", [](NodeRefTemplate<Node>a, float b)
            { return NodeRef(new Subtract(a, NodeRef(new Constant(b)))); })
        .def("__mul__", [](NodeRef a, NodeRef b)
            { return NodeRef(new Multiply(a, b)); })
        .def("__mul__", [](NodeRef a, float b)
            { return NodeRef(new Multiply(a, NodeRef(new Constant(b)))); })
        .def("__rmul__", [](NodeRefTemplate<Node>a, float b)
            { return NodeRef(new Multiply(a, NodeRef(new Constant(b)))); });

    py::class_<Constant, Node, NodeRefTemplate<Constant>>(m, "Constant")
        .def(py::init<double>());

    py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
        .def(py::init<NodeRef>(), "frequency"_a = NodeRef(440.0))
        .def(py::init<float>(),   "frequency"_a = NodeRef(440.0));

    py::class_<Tick, Node, NodeRefTemplate<Tick>>(m, "Tick")
        .def(py::init<NodeRef>(), "frequency"_a = NodeRef(440.0))
        .def(py::init<float>(),   "frequency"_a = NodeRef(440.0));

    py::class_<Dust, Node, NodeRefTemplate<Dust>>(m, "Dust")
        .def(py::init<NodeRef>(), "frequency"_a = NodeRef(440.0))
        .def(py::init<float>(),   "frequency"_a = NodeRef(440.0));

    py::class_<Square, Node, NodeRefTemplate<Square>>(m, "Square")
        .def(py::init<NodeRef, NodeRef>(), "frequency"_a = NodeRef(440.0), "width"_a = NodeRef(0.5))
        .def(py::init<float,   float  >(), "frequency"_a = NodeRef(440.0), "width"_a = NodeRef(0.5));

    py::class_<Wavetable, Node, NodeRefTemplate<Wavetable>>(m, "Wavetable")
        .def(py::init<BufferRef, NodeRef>(), "buffer"_a, "frequency"_a = NodeRef(440.0))
        .def(py::init<BufferRef, float>(),   "buffer"_a, "frequency"_a = NodeRef(440.0));

    py::class_<Pan, Node, NodeRefTemplate<Pan>>(m, "Pan")
        .def(py::init<int, NodeRef, NodeRef>(), "channels"_a = 2, "input"_a = NodeRef(440.0), "pan"_a = NodeRef(0))
        .def(py::init<int, NodeRef, float  >(), "channels"_a = 2, "input"_a = NodeRef(440.0), "pan"_a = NodeRef(0));

    py::class_<ASR, Node, NodeRefTemplate<ASR>>(m, "ASR")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), "attack"_a = NodeRef(0.1), "sustain"_a = NodeRef(0.1), "release"_a = NodeRef(0.1), "clock"_a = NodeRef(0))
        .def(py::init<float,   float,   float,   float  >(), "attack"_a = NodeRef(0.1), "sustain"_a = NodeRef(0.1), "release"_a = NodeRef(0.1), "clock"_a = NodeRef(0));

    py::class_<Sampler, Node, NodeRefTemplate<Sampler>>(m, "Sampler")
        .def(py::init<BufferRef, NodeRef, NodeRef>(), "buffer"_a, "rate"_a = NodeRef(1.0), "loop"_a = NodeRef(1))
        .def(py::init<BufferRef, float,   NodeRef>(), "buffer"_a, "rate"_a = NodeRef(1.0), "loop"_a = NodeRef(1))
        .def(py::init<BufferRef, NodeRef, float  >(), "buffer"_a, "rate"_a = NodeRef(1.0), "loop"_a = NodeRef(1))
        .def(py::init<BufferRef, float,   float  >(), "buffer"_a, "rate"_a = NodeRef(1.0), "loop"_a = NodeRef(1));

    py::class_<Granulator, Node, NodeRefTemplate<Granulator>>(m, "Granulator")
        .def(py::init<BufferRef, NodeRef, NodeRef, NodeRef, NodeRef>(),
            "buffer"_a, "clock"_a = NodeRef(0), "pos"_a = NodeRef(0.5), "grain_length"_a = NodeRef(0.1), "rate"_a = NodeRef(1.0));

    py::class_<AudioGraph>(m, "AudioGraph")
        .def(py::init<>())
        .def("start", &AudioGraph::start)
        .def("wait", [](AudioGraph &graph)
        {
            // Interruptible wait
            // https://pybind11.readthedocs.io/en/stable/faq.html#how-can-i-properly-handle-ctrl-c-in-long-running-functions
            for (;;) {
                if (PyErr_CheckSignals() != 0)
                    throw py::error_already_set();
            }
        })
        .def("add_output", [](AudioGraph &graph, NodeRef a) { graph.add_output(a); });

    py::class_<Buffer, BufferRefTemplate<Buffer>>(m, "Buffer")
        .def(py::init<std::string>())
        .def(py::init<int, int, std::vector<std::vector<float>>>())
        .def_readonly("num_frames", &Buffer::num_frames)
        .def_readonly("num_channels", &Buffer::num_channels)
        .def("data", [](Buffer &buf) {
            return py::array_t<float>({ buf.num_frames }, { sizeof(float) }, buf.data[0]);
        });

    py::class_<InterpolatingBuffer2D, BufferRefTemplate<InterpolatingBuffer2D>>(m, "Buffer2D")
        .def(py::init<BufferRef, BufferRef>());

    py::class_<Wavetable2D, Node, NodeRefTemplate<Wavetable2D>>(m, "Wavetable2D")
        .def(py::init<BufferRef2D, NodeRef, NodeRef>(), "buffer"_a, "frequency"_a = NodeRef(440.0), "crossfade"_a = NodeRef(0));

    py::class_<EnvelopeBufferHanning, Buffer, BufferRefTemplate<EnvelopeBufferHanning>>(m, "EnvelopeBufferHanning")
        .def(py::init<int>());

}

// clang-format on
