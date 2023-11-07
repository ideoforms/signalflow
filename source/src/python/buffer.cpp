#include "signalflow/python/python.h"

void init_python_buffer(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Buffer
     *-------------------------------------------------------------------------------*/
    py::class_<Buffer, BufferRefTemplate<Buffer>>(m, "Buffer", "A buffer of audio samples, containing one or more channels.")
        /*--------------------------------------------------------------------------------
         * Constructors
         *-------------------------------------------------------------------------------*/
        .def(py::init<>(), R"pbdoc(Create a null Buffer with no memory allocated.)pbdoc")
        .def(py::init<std::string>(), "filename"_a, R"pbdoc(Load a Buffer from an audio file.)pbdoc")
        .def(py::init<int, int>(), "num_channels"_a, "num_frames"_a,
             R"pbdoc(Allocate a buffer with `num_channels` channels and `num_frames` frames.)pbdoc")
        .def(
            py::init<int, int, std::vector<std::vector<float>>>(), "num_channels"_a, "num_frames"_a,
            "data"_a
            ","
            R"pbdoc(Allocate a buffer with `num_channels` channels and `num_frames` frames, containing the floating-point samples in `data`.)pbdoc")
        .def(
            py::init<std::vector<std::vector<float>>>(),
            R"pbdoc(Allocate a buffer with `num_channels` channels and `num_frames` frames, containing the floating-point samples in `data`.)pbdoc")
        .def(py::init<std::vector<float>>(), "data"_a,
             R"pbdoc(Allocate a buffer containing the floating-point samples in `data`.)pbdoc")

        /*--------------------------------------------------------------------------------
         * Operators
         *-------------------------------------------------------------------------------*/
        .def("__getitem__",
             [](BufferRef a, unsigned int b) {
                 if (b < 0 || b >= a->get_num_channels())
                 {
                     throw std::runtime_error("Invalid channel index: " + std::to_string(b));
                 }
                 std::vector<sample> a_vec(a->data[b], a->data[b] + a->get_num_frames());
                 BufferRef buffer = new Buffer(a_vec);
                 buffer->set_sample_rate(a->get_sample_rate());
                 return buffer;
             })
        .def("__str__",
             [](BufferRef a) {
                 std::string filename = a->get_filename();
                 if (filename.empty())
                 {
                     return "Buffer (" + std::to_string(a->get_num_channels()) + " channels, "
                         + std::to_string(a->get_num_frames()) + " frames)";
                 }
                 else
                 {
                     return "Buffer (" + filename + ", " + std::to_string(a->get_num_channels()) + " channels, "
                         + std::to_string(a->get_num_frames()) + " frames)";
                 }
             })
        .def(
            "__mul__", [](BufferRef a, float b) { return a * b; }, "value"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` multiplied by the scaling factor `value`.)pbdoc")
        .def(
            "__rmul__", [](BufferRef a, float b) { return a * b; }, "value_"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` multiplied by the scaling factor `value`.)pbdoc")
        .def(
            "__len__", [](Buffer &buf) { return buf.get_num_frames(); },
            R"pbdoc(Returns the length of the buffer `self`, in frames.)pbdoc")

        /*--------------------------------------------------------------------------------
         * Properties
         *-------------------------------------------------------------------------------*/
        .def_property_readonly("num_frames", &Buffer::get_num_frames,
                               R"pbdoc(Returns the number of frames the buffer.)pbdoc")
        .def_property_readonly("num_channels", &Buffer::get_num_channels,
                               R"pbdoc(Returns the number of channels in the buffer.)pbdoc")
        .def_property_readonly("sample_rate", &Buffer::get_sample_rate,
                               R"pbdoc(Returns the buffer's sample rate.)pbdoc")
        .def_property_readonly("duration", &Buffer::get_duration,
                               R"pbdoc(Returns the buffer's duration, in seconds.)pbdoc")
        .def_property_readonly("filename", &Buffer::get_filename,
                               R"pbdoc(Returns the buffer's filename, if the buffer has been loaded from/saved to file.)pbdoc")
        .def_property("interpolate", &Buffer::get_interpolation_mode, &Buffer::set_interpolation_mode,
                      R"pbdoc(Get/set the buffer's interpolation mode.)pbdoc")

#if !(defined(__linux__) && defined(__arm__))
        // This does not compile on Raspberry Pi OS (2022-08) - to fix
        .def_property_readonly("data",
                               [](Buffer &buf) {
                                   /*--------------------------------------------------------------------------------
                                    * Assigning a data owner to the array ensures that it is returned as a
                                    * pointer to the original data, rather than a copy. This means that we can
                                    * modify the contents of the output buffer in-place from Python if we want to.
                                    * https://github.com/pybind/pybind11/issues/323
                                    *-------------------------------------------------------------------------------*/
                                   py::str dummy_data_owner;
                                   return py::array_t<float>(
                                       { (int) buf.get_num_channels(), (int) buf.get_num_frames() },
                                       { sizeof(float) * buf.get_num_frames(), sizeof(float) }, buf.data[0],
                                       dummy_data_owner);
                               })
#endif

        /*--------------------------------------------------------------------------------
         * Methods
         *-------------------------------------------------------------------------------*/
        .def("get", &Buffer::get, "channel"_a, "frame"_a)
        .def("get_frame", &Buffer::get_frame, "channel"_a, "frame"_a)
        .def(
            "fill", [](Buffer &buf, float sample) { buf.fill(sample); }, "sample"_a)
        .def(
            "fill", [](Buffer &buf, const std::function<float(float)> f) { buf.fill(f); }, "function"_a)
        .def("split", &Buffer::split, "num_frames_per_part"_a)
        .def("load", &Buffer::load, "filename"_a)
        .def("save", &Buffer::save, "filename"_a);

    py::class_<Buffer2D, Buffer, BufferRefTemplate<Buffer2D>>(m, "Buffer2D", "Two-dimensional buffer of audio samples")
        .def(py::init<std::vector<BufferRef>>())
        .def("get2D", &Buffer2D::get2D);

    py::class_<WaveShaperBuffer, Buffer, BufferRefTemplate<WaveShaperBuffer>>(m, "WaveShaperBuffer", "Sample buffer for waveshaper nodes")
        .def(py::init<int>());

    py::class_<EnvelopeBuffer, Buffer, BufferRefTemplate<EnvelopeBuffer>>(m, "EnvelopeBuffer", "Buffer encapsulating an audio envelope")
        .def(py::init<int>())
        .def(py::init<std::string>())
        .def(py::init<std::string, int>());
}
