#include "signal/python/python.h"

void init_python_synth(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Synth
     *-------------------------------------------------------------------------------*/
    py::class_<Synth, SynthRefTemplate<Synth>>(m, "Synth")
        .def(py::init<SynthSpecRef>())
        .def(py::init<>())
        .def("set_input", &Synth::set_input)
        .def_property("auto_free", &Synth::get_auto_free, &Synth::set_auto_free)
        .def_readonly("output", &Synth::output)
        .def_readonly("nodes", &Synth::nodes)
        .def_readonly("inputs", &Synth::inputs)

        // template methods
        .def("add_input", &Synth::add_input)
        .def("add_node", &Synth::add_node)
        .def("set_output", &Synth::set_output)
        .def("parse", &Synth::parse);

    py::class_<SynthSpec, SynthSpecRefTemplate<SynthSpec>>(m, "SynthSpec")
        .def("print", [](SynthSpec &synthspec) { synthspec.print(); })
        .def_readonly("name", &SynthSpec::name);
}
