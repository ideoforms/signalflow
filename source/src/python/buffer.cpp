#include "signal/python/python.h"

void init_python_buffer(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Buffer
     *-------------------------------------------------------------------------------*/
    py::class_<Buffer, BufferRefTemplate<Buffer>>(m, "Buffer")
        .def(py::init<std::string>())
        .def(py::init<int, int>())
        .def(py::init<int, int, std::vector<std::vector<float>>>())
        .def_readonly("num_frames", &Buffer::num_frames)
        .def_readonly("num_channels", &Buffer::num_channels)
        .def("load", &Buffer::load)
        .def("data", [](Buffer &buf) {
            return py::array_t<float>({ buf.num_frames }, { sizeof(float) }, buf.data[0]);
        });

    py::class_<Buffer2D, BufferRefTemplate<Buffer2D>>(m, "Buffer2D")
        .def(py::init<BufferRef, BufferRef>());

    py::class_<EnvelopeBufferHanning, Buffer, BufferRefTemplate<EnvelopeBufferHanning>>(m, "EnvelopeBufferHanning")
        .def(py::init<int>());
}
