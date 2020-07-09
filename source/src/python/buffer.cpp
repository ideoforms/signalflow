#include "signal/python/python.h"

void init_python_buffer(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Buffer
     *-------------------------------------------------------------------------------*/
    py::class_<Buffer, BufferRefTemplate<Buffer>>(m, "Buffer", "A buffer of audio samples")
        .def(py::init<>())
        .def(py::init<std::string>())
        .def(py::init<int, int>(), R"pbdoc(
            Init
            ----
        )pbdoc")
        .def(py::init<int, int, std::vector<std::vector<float>>>())
        .def(py::init<std::vector<std::vector<float>>>())
        .def(py::init<std::vector<float>>())
        .def("__getitem__", [](BufferRef a, int b) {
            if (b < 0 || b >= a->num_channels)
            {
                throw std::runtime_error("Invalid channel index: " + std::to_string(b));
            }
            std::vector<sample> a_vec(a->data[b], a->data[b] + a->num_frames);
            return new Buffer(a_vec);
        })
        .def("__str__", [](BufferRef a) {
            return "Buffer (" + std::to_string(a->num_channels) + " channels, " + std::to_string(a->num_frames) + " frames)";
        })
        .def("__mul__", [](BufferRef a, float b) { return a * b; })
        .def("__rmul__", [](BufferRef a, float b) { return a * b; })

        .def("__len__", [](Buffer &buf) { return buf.num_frames; })

        .def_readonly("num_frames", &Buffer::num_frames)
        .def_readonly("num_channels", &Buffer::num_channels)
        .def_readonly("sample_rate", &Buffer::sample_rate)
        .def_readonly("duration", &Buffer::duration)
        .def_readwrite("interpolate", &Buffer::interpolate)

        .def("split", &Buffer::split)
        .def("get", &Buffer::get)
        .def("get_frame", &Buffer::get_frame)
        .def("fill", [](Buffer &buf, float sample) { buf.fill(sample); })

        .def("load", &Buffer::load)
        .def("save", &Buffer::save)
        .def_property_readonly("data", [](Buffer &buf) {
            return py::array_t<float>(
                { buf.num_channels, buf.num_frames },
                { sizeof(float) * buf.num_frames, sizeof(float) },
                buf.data[0]);
        });

    py::class_<Buffer2D, Buffer, BufferRefTemplate<Buffer2D>>(m, "Buffer2D")
        .def(py::init<std::vector<BufferRef>>())
        .def("get2D", &Buffer2D::get2D);

    py::class_<EnvelopeBufferHanning, Buffer, BufferRefTemplate<EnvelopeBufferHanning>>(m, "EnvelopeBufferHanning")
        .def(py::init<int>());
}
