#include "signalflow/python/python.h"

void init_python_buffer(py::module &m)
{
    py::class_<SampleRingBuffer>(m, "SampleRingBuffer", "A circular buffer of audio samples with a single read/write head")
        .def(py::init<unsigned int>(), "capacity"_a, R"pbdoc(Create a new ring buffer)pbdoc")
        .def("append", &SampleRingBuffer::append, R"pbdoc(Append an item to the ring buffer.)pbdoc")
        .def(
            "extend", [](SampleRingBuffer &buf, std::vector<sample> vec) { buf.extend(vec); },
            R"pbdoc(Extend the ring buffer.)pbdoc")
        .def(
            "get", [](SampleRingBuffer &buf, int index) { return buf.get(index); },
            R"pbdoc(Retrieve an item from the ring buffer, with offset relative to the read head.)pbdoc")
        .def(
            "get", [](SampleRingBuffer &buf, float index) { return buf.get(index); },
            R"pbdoc(Retrieve an item from the ring buffer, with offset relative to the read head.)pbdoc")
        .def("get_capacity", &SampleRingBuffer::get_capacity, R"pbdoc(Returns the capacity of the ring buffer.)pbdoc");

    py::class_<SampleRingQueue>(m, "SampleRingQueue", "A circular queue of audio samples with separate read/write heads")
        .def(py::init<unsigned int>(), "capacity"_a, R"pbdoc(Create a new ring queue)pbdoc")
        .def("append", &SampleRingQueue::append, R"pbdoc(Append an item to the ring queue.)pbdoc")
        .def(
            "extend", [](SampleRingQueue &buf, std::vector<sample> vec) { buf.extend(vec); },
            R"pbdoc(Extend the ring queue.)pbdoc")
        .def("pop", &SampleRingQueue::pop, R"pbdoc(Pop an item from the ring queue.)pbdoc")
        .def("get_capacity", &SampleRingQueue::get_capacity, R"pbdoc(Returns the capacity of the ring queue.)pbdoc")
        .def("get_filled_count", &SampleRingQueue::get_filled_count, R"pbdoc(Returns the number of items filled in the ring queue.)pbdoc");

    /*--------------------------------------------------------------------------------
     * Buffer
     *-------------------------------------------------------------------------------*/
    py::class_<Buffer, BufferRefTemplate<Buffer>>(m, "Buffer",
                                                  "A buffer of audio samples, containing one or more channels.")
        /*--------------------------------------------------------------------------------
         * Constructors
         *-------------------------------------------------------------------------------*/
        .def(py::init<>(), R"pbdoc(Create a null Buffer with no memory allocated.)pbdoc")
        .def(py::init<std::string>(), "filename"_a, R"pbdoc(Load a Buffer from an audio file.)pbdoc")
        .def(py::init<int, int>(), "num_channels"_a, "num_frames"_a,
             R"pbdoc(Allocate a buffer with `num_channels` channels and `num_frames` frames.)pbdoc")
        .def(
            py::init<int, int, std::vector<std::vector<float>>>(), "num_channels"_a, "num_frames"_a, "data"_a,
            R"pbdoc(Allocate a buffer with `num_channels` channels and `num_frames` frames, containing the floating-point samples in `data`.)pbdoc")
        .def(
            py::init<std::vector<std::vector<float>>>(),
            R"pbdoc(Allocate a buffer with `num_channels` channels and `num_frames` frames, containing the floating-point samples in `data`.)pbdoc")
        .def(py::init<std::vector<float>>(), "data"_a,
             R"pbdoc(Allocate a buffer containing the floating-point samples in `data`.)pbdoc")

        .def(py::init<const std::function<float(float)>>(), "function"_a,
             R"pbdoc(Allocate a buffer filled with the output of the function `function`.)pbdoc")
        .def(py::init<int, const std::function<float(float)>>(), "num_frames"_a, "function"_a,
             R"pbdoc(Allocate a mono buffer with `num_frames` frames, filled with the output of the function `function`.)pbdoc")
        .def(py::init<int, int, const std::function<float(float)>>(), "num_channels"_a, "num_frames"_a, "function"_a,
             R"pbdoc(Allocate a buffer with `num_channels` channels and `num_frames` frames, filled with the output of the function `function`.)pbdoc")

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
                 std::string channels_s = a->get_num_channels() == 1 ? "" : "s";
                 std::string frames_s = a->get_num_frames() == 1 ? "" : "s";
                 if (filename.empty())
                 {
                     return "Buffer (" + std::to_string(a->get_num_channels()) + " channel" + channels_s + ", "
                         + std::to_string(a->get_num_frames()) + " frame" + frames_s + ")";
                 }
                 else
                 {
                     return "Buffer (" + filename + ", " + std::to_string(a->get_num_channels()) + " channel" + channels_s + ", "
                         + std::to_string(a->get_num_frames()) + " frame" + frames_s + ")";
                 }
             })
        .def(
            "__mul__", [](BufferRef a, float b) { return a * b; }, "value"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` multiplied by `value`.)pbdoc")
        .def(
            "__rmul__", [](BufferRef a, float b) { return a * b; }, "value_"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` multiplied by `value`.)pbdoc")
        .def(
            "__div__", [](BufferRef a, float b) { return a / b; }, "value"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` divided by `value`.)pbdoc")
        .def(
            "__truediv__", [](BufferRef a, float b) { return a / b; }, "value"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` divided by `value`.)pbdoc")
        .def(
            "__add__", [](BufferRef a, float b) { return a + b; }, "value"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` added to `value`.)pbdoc")
        .def(
            "__radd__", [](BufferRef a, float b) { return a + b; }, "value_"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` added to `value`.)pbdoc")
        .def(
            "__sub__", [](BufferRef a, float b) { return a - b; }, "value"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` subtracted by `value`.)pbdoc")
        .def(
            "__len__", [](Buffer &buf) { return buf.get_num_frames(); },
            R"pbdoc(Returns the length of the buffer `self`, in frames.)pbdoc")
        .def(
            "__mul__", [](BufferRef a, BufferRef b) { return a * b; }, "buffer"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` multiplied by `buffer`.)pbdoc")
        .def(
            "__truediv__", [](BufferRef a, BufferRef b) { return a / b; }, "buffer"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` divided by `buffer`.)pbdoc")
        .def(
            "__add__", [](BufferRef a, BufferRef b) { return a + b; }, "buffer"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` added to `buffer`.)pbdoc")
        .def(
            "__sub__", [](BufferRef a, BufferRef b) { return a - b; }, "buffer"_a,
            R"pbdoc(Returns a new Buffer containing the samples in `self` subtracted by `buffer`.)pbdoc")

        /*--------------------------------------------------------------------------------
         * Properties
         *-------------------------------------------------------------------------------*/
        .def_property_readonly("num_frames", &Buffer::get_num_frames,
                               R"pbdoc(Returns the number of frames the buffer.)pbdoc")
        .def_property_readonly("num_channels", &Buffer::get_num_channels,
                               R"pbdoc(Returns the number of channels in the buffer.)pbdoc")
        .def_property("sample_rate", &Buffer::get_sample_rate, &Buffer::set_sample_rate,
                      R"pbdoc(Get/set the buffer's sample rate. Note that this does not resample the audio.)pbdoc")
        .def_property_readonly("duration", &Buffer::get_duration,
                               R"pbdoc(Returns the buffer's duration, in seconds.)pbdoc")
        .def_property_readonly(
            "filename", &Buffer::get_filename,
            R"pbdoc(Returns the buffer's filename, if the buffer has been loaded from/saved to file.)pbdoc")
        .def_property("interpolation_mode", &Buffer::get_interpolation_mode, &Buffer::set_interpolation_mode,
                      R"pbdoc(Get/set the buffer's interpolation mode.)pbdoc")
        .def_property_readonly("frame_offsets", &Buffer::get_frame_offsets,
                               R"pbdoc(Returns a list containing all frame numbers")pbdoc")

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
        .def("set", &Buffer::set, "channel"_a, "frame"_a, "value"_a)
        .def(
            "fill", [](Buffer &buf, float sample) { buf.fill(sample); }, "sample"_a)
        .def(
            "fill", [](Buffer &buf, const std::function<float(float)> f) { buf.fill(f); }, "function"_a)
        .def("split", &Buffer::split, "num_frames_per_part"_a)
        .def("load", &Buffer::load, "filename"_a)
        .def("save", &Buffer::save, "filename"_a)
        .def(
            "play", [](BufferRef &buf, bool blocking = false) {
                AudioGraph *shared_graph = AudioGraph::get_shared_graph();
                if (!shared_graph)
                {
                    throw std::runtime_error("Cannot play buffer: no shared AudioGraph instance");
                }

                PatchRef patch = new Patch();
                NodeRef player = new BufferPlayer(buf);
                patch->add_node(player);
                patch->set_output(player);
                patch->set_auto_free_node(player);
                shared_graph->play(patch);

                if (blocking)
                {
                    shared_graph->wait(buf->get_duration());
                }
            },
            "blocking"_a = false, R"pbdoc(Play the buffer through the default audio output device.)pbdoc")

        /*--------------------------------------------------------------------------------
         * Static methods
         *-------------------------------------------------------------------------------*/
        .def_static("load_directory", &Buffer::load_directory, "directory_path"_a, "extensions"_a = std::vector<std::string>({ "wav", "aif" }), R"pbdoc(list[Buffer]: Load a directory of audio files into a list of buffers.)pbdoc");

    py::class_<Buffer2D, Buffer, BufferRefTemplate<Buffer2D>>(m, "Buffer2D", "Two-dimensional buffer of audio samples")
        .def(py::init<std::vector<BufferRef>>())
        .def("get2D", &Buffer2D::get2D);

    py::class_<WaveShaperBuffer, Buffer, BufferRefTemplate<WaveShaperBuffer>>(m,
                                                                              "WaveShaperBuffer",
                                                                              "Sample buffer for waveshaper nodes")
        .def(py::init<int>(), "num_frames"_a,
             R"pbdoc(Create a waveshaper buffer containing the given number of samples.)pbdoc")
        .def(py::init<const std::function<float(float)>>(), "function"_a,
             R"pbdoc(Create a waveshaper buffer filled with the output of a given function.)pbdoc")
        .def("get", [](WaveShaperBuffer &buf, float sample) { return buf.get(0, sample); })
        .def_property_readonly("frame_offsets", &Buffer::get_frame_offsets,
                               R"pbdoc(Returns a list containing the offset in the waveshaper buffer for each frame, ranging over -1..1.)pbdoc");

    py::class_<EnvelopeBuffer, Buffer, BufferRefTemplate<EnvelopeBuffer>>(m,
                                                                          "EnvelopeBuffer",
                                                                          "Buffer encapsulating an audio envelope")
        .def(py::init<int>(), "num_frames"_a,
             R"pbdoc(Create an envelope buffer containing the given number of samples.)pbdoc")
        .def(py::init<std::vector<float>>(), "samples"_a,
             R"pbdoc(Create an envelope buffer containing the specified 1D array of samples.)pbdoc")
        .def(py::init<std::string>(), "shape"_a,
             R"pbdoc(Create an envelope buffer with the specified shape, one of: rectangular, triangle, hanning, linear-decay.)pbdoc")
        .def(py::init<std::string, int>(), "shape"_a, "num_frames"_a,
             R"pbdoc(Create an envelope buffer with the specified shape and number of frames.)pbdoc")
        .def(py::init<const std::function<float(float)>>(), "function"_a,
             R"pbdoc(Create an envelope buffer filled with the output of a given function.)pbdoc")
        .def_property_readonly("frame_offsets", &Buffer::get_frame_offsets,
                               R"pbdoc(Returns a list containing the offset in the envelope buffer for each frame, ranging over 0..1.)pbdoc");

    py::class_<WavetableBuffer, Buffer, BufferRefTemplate<WavetableBuffer>>(m,
                                                                            "WavetableBuffer",
                                                                            "Buffer encapsulating a single cycle of a wavetable")
        .def(py::init<int>(), "num_frames"_a,
             R"pbdoc(Create a wavetable buffer containing the given number of samples.)pbdoc")
        .def(py::init<std::vector<float>>(), "samples"_a,
             R"pbdoc(Create n wavetable buffer containing the specified 1D array of samples.)pbdoc")
        .def(py::init<std::string>(), "shape"_a,
             R"pbdoc(Create a wavetable buffer with the specified shape, one of: rectangular, triangle, hanning, linear-decay.)pbdoc")
        .def(py::init<std::string, int>(), "shape"_a, "num_frames"_a,
             R"pbdoc(Create a wavetable buffer with the specified shape and number of frames.)pbdoc")
        .def(py::init<const std::function<float(float)>>(), "function"_a,
             R"pbdoc(Create a wavetable buffer filled with the output of a given function.)pbdoc")
        .def_property_readonly("frame_offsets", &Buffer::get_frame_offsets,
                               R"pbdoc(Returns a list containing the offset in the wavetable buffer for each frame, ranging over 0..1.)pbdoc");

    /*--------------------------------------------------------------------------------
     * FFTBuffer
     *-------------------------------------------------------------------------------*/
    py::class_<FFTBuffer, FFTBufferRefTemplate<FFTBuffer>>(m, "FFTBuffer",
                                                           "A buffer of audio spectra in magnitude/phase format")
        /*--------------------------------------------------------------------------------
         * Constructors
         *-------------------------------------------------------------------------------*/
        .def(py::init<std::string, int, int>(), "filename"_a, "fft_size"_a, "hop_size"_a, R"pbdoc(Load an FFTBuffer from a .spectra file.)pbdoc")

        /*--------------------------------------------------------------------------------
         * Operators
         *-------------------------------------------------------------------------------*/
        .def("__str__",
             [](FFTBufferRef a) {
                 std::string filename = a->get_filename();
                 if (filename.empty())
                 {
                     return "FFTBuffer (" + std::to_string(a->get_num_frames()) + " frames)";
                 }
                 else
                 {
                     return "FFTBuffer (" + filename + ", " + std::to_string(a->get_num_frames()) + " frames)";
                 }
             })

        /*--------------------------------------------------------------------------------
         * Properties
         *-------------------------------------------------------------------------------*/
        .def_property_readonly("num_frames", &FFTBuffer::get_num_frames,
                               R"pbdoc(Returns the number of spectral frames in the FFT buffer.)pbdoc")
        .def_property_readonly("sample_rate", &FFTBuffer::get_sample_rate,
                               R"pbdoc(Returns the FFT buffer's sample rate.)pbdoc")
        .def_property_readonly("duration", &FFTBuffer::get_duration,
                               R"pbdoc(Returns the FFT buffer's duration, in seconds.)pbdoc")
        .def_property_readonly(
            "filename", &FFTBuffer::get_filename,
            R"pbdoc(Returns the FFT buffer's filename, if the buffer has been loaded from/saved to file.)pbdoc");
}
