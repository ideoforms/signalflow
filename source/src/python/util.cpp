#include "signal/python/python.h"

void init_python_util(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Utils
     *-------------------------------------------------------------------------------*/
    m.def("signal_clip", signal_clip, R"pbdoc(Clip arg0 between [arg1..arg2])pbdoc");
    m.def("signal_scale_lin_lin", signal_scale_lin_lin, R"pbdoc(Scale arg0, from linear range [arg1..arg2] to linear range [arg3..arg4])pbdoc");
    m.def("signal_scale_lin_exp", signal_scale_lin_exp, R"pbdoc(Scale arg0, from linear range [arg1..arg2] to exponential range [arg3..arg4])pbdoc");
    m.def("signal_scale_exp_lin", signal_scale_exp_lin, R"pbdoc(Scale arg0, from exponential range [arg1..arg2] to linear range [arg3..arg4])pbdoc");

    m.def("signal_frequency_to_midi_note", signal_frequency_to_midi_note, R"pbdoc(Map frequency in Hz to a MIDI note value)pbdoc");
    m.def("signal_midi_note_to_frequency", signal_midi_note_to_frequency, R"pbdoc(Map MIDI note value to a frequency in Hz)pbdoc");

    m.def("signal_db_to_amp", signal_db_to_amp, R"pbdoc(Map decibels to linear amplitude)pbdoc");
    m.def("signal_amp_to_db", signal_amp_to_db, R"pbdoc(Map linear amplitude to decibels)pbdoc");

    m.def("signal_save_block_to_text_file", signal_save_block_to_text_file, R"pbdoc(Write a block of PCM float samples to a .csv-style text file)pbdoc");
    m.def("signal_save_block_to_wav_file", signal_save_block_to_wav_file, R"pbdoc(Write a block of PCM float samples to a .wav file)pbdoc");
}
