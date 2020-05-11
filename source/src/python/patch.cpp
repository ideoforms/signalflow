#include "signal/python/python.h"

void init_python_patch(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Patch
     *-------------------------------------------------------------------------------*/
    py::class_<Patch, PatchRefTemplate<Patch>>(m, "Patch")
        .def(py::init<PatchSpecRef>())
        .def(py::init<>())
        .def("set_input", &Patch::set_input)
        .def_property("auto_free", &Patch::get_auto_free, &Patch::set_auto_free)
        .def_readonly("output", &Patch::output)
        .def_readonly("nodes", &Patch::nodes)
        .def_readonly("inputs", &Patch::inputs)

        // template methods
        .def("add_input", &Patch::add_input)
        .def("add_node", &Patch::add_node)
        .def("set_output", &Patch::set_output)
        .def("create_spec", &Patch::create_spec);

    py::class_<PatchSpec, PatchSpecRefTemplate<PatchSpec>>(m, "PatchSpec")
        .def("print", [](PatchSpec &patchspec) { patchspec.print(); })
        .def_readonly("name", &PatchSpec::name);
}
