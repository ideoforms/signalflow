#include "signalflow/python/python.h"

void init_python_buffer(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Buffer
     *-------------------------------------------------------------------------------*/
    py::class_<Buffer, BufferRefTemplate<Buffer>>(m, "Buffer", "A buffer of audio samples")
        .def(py::init<>())
        .def(py::init<std::string>())
        .def(py::init<std::string, int>())
        .def(py::init<int, int>(), R"pbdoc(
            Init
            ----
        )pbdoc")
        .def(py::init<int, int, std::vector<std::vector<float>>>())
        .def(py::init<std::vector<std::vector<float>>>())
        .def(py::init<std::vector<float>>())
        .def("__getitem__", [](BufferRef a, int b) {
            if (b < 0 || b >= a->get_num_channels())
            {
                throw std::runtime_error("Invalid channel index: " + std::to_string(b));
            }
            std::vector<sample> a_vec(a->data[b], a->data[b] + a->get_num_frames());
            return new Buffer(a_vec);
        })
        .def("__str__", [](BufferRef a) {
            return "Buffer (" + std::to_string(a->get_num_channels()) + " channels, " + std::to_string(a->get_num_frames()) + " frames)";
        })
        .def("__mul__", [](BufferRef a, float b) { return a * b; })
        .def("__rmul__", [](BufferRef a, float b) { return a * b; })

        .def("__len__", [](Buffer &buf) { return buf.get_num_frames(); })

        .def_property_readonly("num_frames", &Buffer::get_num_frames)
        .def_property_readonly("num_channels", &Buffer::get_num_channels)
        .def_property_readonly("sample_rate", &Buffer::get_sample_rate)
        .def_property_readonly("duration", &Buffer::get_duration)
        .def_property("interpolate", &Buffer::get_interpolation_mode, &Buffer::set_interpolation_mode)

        .def("split", &Buffer::split)
        .def("get", &Buffer::get)
        .def("get_frame", &Buffer::get_frame)
        .def("fill", [](Buffer &buf, float sample) { buf.fill(sample); })
        .def("fill", [](Buffer &buf, const std::function<float(float)> f) { buf.fill(f); })
        .def("load", &Buffer::load)
        .def("save", &Buffer::save)
        .def_property_readonly("data", [](Buffer &buf) {
            /*--------------------------------------------------------------------------------
             * Assigning a data owner to the array ensures that it is returned as a
             * pointer to the original data, rather than a copy. This means that we can
             * modify the contents of the output buffer in-place from Python if we want to.
             * https://github.com/pybind/pybind11/issues/323
             *-------------------------------------------------------------------------------*/
            py::str dummy_data_owner;
            return py::array_t<float>(
                { buf.get_num_channels(), buf.get_num_frames() },
                { sizeof(float) * buf.get_num_frames(), sizeof(float) },
                buf.data[0],
                dummy_data_owner);
        });

    py::class_<Buffer2D, Buffer, BufferRefTemplate<Buffer2D>>(m, "Buffer2D")
        .def(py::init<std::vector<BufferRef>>())
        .def("get2D", &Buffer2D::get2D);

    py::class_<WaveShaperBuffer, Buffer, BufferRefTemplate<WaveShaperBuffer>>(m, "WaveShaperBuffer")
        .def(py::init<int>());
}
