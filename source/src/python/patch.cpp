#include "signalflow/python/python.h"

void init_python_patch(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Patch
     *-------------------------------------------------------------------------------*/
    py::class_<Patch, PatchRefTemplate<Patch>>(m, "Patch")
        .def(py::init<PatchSpecRef>())
        .def(py::init<PatchRef>())
        .def(py::init<>())
        .def("set_input", [](Patch &patch, std::string name, float value) { patch.set_input(name, value); })
        .def("set_input", [](Patch &patch, std::string name, NodeRef node) { patch.set_input(name, node); })
        .def("set_input", [](Patch &patch, std::string name, BufferRef buffer) { patch.set_input(name, buffer); })
        .def_property("auto_free", &Patch::get_auto_free, &Patch::set_auto_free)
        .def_readonly("output", &Patch::output)
        .def_readonly("nodes", &Patch::nodes)
        .def_readonly("inputs", &Patch::inputs)

        // template methods
        .def("add_input", [](Patch &patch, std::string name) { return patch.add_input(name); })
        .def("add_input", [](Patch &patch, std::string name, float value) { return patch.add_input(name, value); })
        .def("add_input", &Patch::add_input)
        .def("add_buffer_input", &Patch::add_buffer_input)
        .def("add_node", &Patch::add_node)
        .def("set_output", &Patch::set_output)
        .def("create_spec", &Patch::create_spec);

    py::class_<PatchSpec, PatchSpecRefTemplate<PatchSpec>>(m, "PatchSpec")
        .def(py::init<std::string>())
        .def_property_readonly("name", &PatchSpec::get_name)
        .def("print", [](PatchSpec &patchspec) { patchspec.print(); })
        .def("load", &PatchSpec::load)
        .def("save", &PatchSpec::save);

    py::class_<PatchRegistry>(m, "PatchRegistry")
        .def(py::init(&PatchRegistry::global))
        .def("create", &PatchRegistry::create);
}
