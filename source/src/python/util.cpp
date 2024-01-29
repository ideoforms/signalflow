#include "signalflow/python/python.h"

void init_python_util(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Utils
     *-------------------------------------------------------------------------------*/
    m.def("clip", signalflow_clip, R"pbdoc(Clip arg0 between [arg1..arg2])pbdoc");
    m.def("wrap", signalflow_wrap, R"pbdoc(Wrap arg0 between [arg1..arg2])pbdoc");
    m.def("fold", signalflow_fold, R"pbdoc(Fold arg0 between [arg1..arg2])pbdoc");
    m.def("scale_lin_lin", signalflow_scale_lin_lin, R"pbdoc(Scale arg0, from linear range [arg1..arg2] to linear range [arg3..arg4])pbdoc");
    m.def("scale_lin_exp", signalflow_scale_lin_exp, R"pbdoc(Scale arg0, from linear range [arg1..arg2] to exponential range [arg3..arg4])pbdoc");
    m.def("scale_exp_lin", signalflow_scale_exp_lin, R"pbdoc(Scale arg0, from exponential range [arg1..arg2] to linear range [arg3..arg4])pbdoc");

    m.def("frequency_to_midi_note", signalflow_frequency_to_midi_note, R"pbdoc(Map frequency in Hz to a MIDI note value)pbdoc");
    m.def("midi_note_to_frequency", signalflow_midi_note_to_frequency, R"pbdoc(Map MIDI note value to a frequency in Hz)pbdoc");

    m.def("db_to_amplitude", signalflow_db_to_amplitude, R"pbdoc(Map decibels to linear amplitude)pbdoc");
    m.def("amplitude_to_db", signalflow_amplitude_to_db, R"pbdoc(Map linear amplitude to decibels)pbdoc");

    m.def("save_block_to_text_file", signalflow_save_block_to_text_file, R"pbdoc(Write a block of PCM float samples to a .csv-style text file)pbdoc");
    m.def("save_block_to_wav_file", signalflow_save_block_to_wav_file, R"pbdoc(Write a block of PCM float samples to a .wav file)pbdoc");

    m.def("random_seed", random_seed, R"pbdoc(Set the global random seed)pbdoc");
    m.def(
        "random_uniform", []() { return random_uniform(); }, R"pbdoc(Return a uniformly random number)pbdoc");
    m.def(
        "random_uniform", [](float from, float to) { return random_uniform(from, to); }, R"pbdoc(Return a uniformly random number)pbdoc");
    m.def(
        "random_exponential", [](float from, float to) { return random_exponential(from, to); }, R"pbdoc(Return a random number, exponentially distributed across a fixed range)pbdoc");

    py::class_<KDTree>(m, "KDTree", "A KDTree structure")
        .def(py::init<std::vector<std::vector<float>>>(), "data"_a = nullptr)
        .def("get_nearest", &KDTree::get_nearest, "target"_a);

    py::class_<KDTreeMatch>(m, "KDTreeMatch", "A KDTreeMatch result")
        .def_property_readonly("index", &KDTreeMatch::get_index, R"pbdoc(The index)pbdoc")
        .def_property_readonly("coordinate", &KDTreeMatch::get_coordinate, R"pbdoc(The coordinate)pbdoc")
        .def_property_readonly("distance", &KDTreeMatch::get_distance, R"pbdoc(The distance)pbdoc");
}
