#include "signal/python/python.h"

void init_python_synth(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Synth
     *-------------------------------------------------------------------------------*/
    py::class_<SynthTemplate, SynthTemplateRefTemplate<SynthTemplate>>(m, "SynthTemplate")
        .def(py::init<std::string>())
        .def("add_input", &SynthTemplate::add_input)
        .def("add_node", &SynthTemplate::add_node)
        .def("set_output", &SynthTemplate::set_output)
        .def("parse", &SynthTemplate::parse);

    py::class_<Synth, SynthRefTemplate<Synth>>(m, "Synth")
        .def(py::init<SynthTemplateRef>())
        .def(py::init<SynthSpecRef>())
        .def("set_input", [](Synth &synth, std::string name, float value)
        {
            synth.set_input(name, value);
        })
        .def("set_input", [](Synth &synth, std::string name, NodeRef value)
        {
            synth.set_input(name, value);
        })
        .def_readonly("output", &Synth::output)
        .def_readonly("nodes", &Synth::nodes)
        .def_readonly("inputs", &Synth::inputs);

    py::class_<SynthSpec, SynthSpecRefTemplate<SynthSpec>>(m, "SynthSpec")
        .def("print", [](SynthSpec &synthspec) { synthspec.print(); })
        .def_readonly("name", &SynthSpec::name);
}
