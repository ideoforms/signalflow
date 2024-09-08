#include "signalflow/python/python.h"

void init_python_patch(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Patch
     *-------------------------------------------------------------------------------*/
    py::class_<Patch, PatchRefTemplate<Patch>>(m, "Patch", "A Patch encapsulates a connected network of nodes, analogous to a synthesizer")
        /*--------------------------------------------------------------------------------
         * Constructors
         *-------------------------------------------------------------------------------*/
        .def(py::init<PatchSpecRef, std::map<std::string, NodeRef>>())
        .def(py::init<PatchSpecRef>())
        .def(py::init<>())

        /*--------------------------------------------------------------------------------
         * Operators
         *-------------------------------------------------------------------------------*/
        .def("__mul__", [](PatchRef a, NodeRef b) { return a * b; })
        .def("__mul__", [](PatchRef a, float b) { return a * NodeRef(b); })
        .def("__rmul__", [](PatchRef a, float b) { return a * NodeRef(b); })
        .def("__add__", [](PatchRef a, NodeRef b) { return a + b; })
        .def("__add__", [](PatchRef a, float b) { return a + NodeRef(b); })
        .def("__radd__", [](PatchRef a, float b) { return a + NodeRef(b); })
        .def("__sub__", [](PatchRef a, NodeRef b) { return a - b; })
        .def("__sub__", [](PatchRef a, float b) { return a - NodeRef(b); })
        // TODO: Implement this properly
        .def("__rsub__", [](PatchRef a, float b) { return new Subtract(NodeRef(b), a->get_output()); })
        .def("__truediv__", [](PatchRef a, NodeRef b) { return a / b; })
        .def("__truediv__", [](PatchRef a, float b) { return a / NodeRef(b); })
        .def("__rtruediv__", [](PatchRef a, float b) { return new Divide(NodeRef(b), a->get_output()); })

        // Breaks other properties (auto_free, inputs, etc).
        // Need a policy on this: either *only* inputs should be accessible through properties,
        // or inputs should all only be accessible through set_input(...).
        // Ideally, `obj.input = x` should call set_input iff .input is a pre-existing input.
        // .def("__setattr__", [](PatchRef a, std::string attr, NodeRef value) { a->set_input(attr, value); })
        .def("set_input", [](Patch &patch, std::string name, float value) { patch.set_input(name, value); })
        .def("set_input", [](Patch &patch, std::string name, NodeRef node) { patch.set_input(name, node); })
        .def("set_input", [](Patch &patch, std::string name, BufferRef buffer) { patch.set_input(name, buffer); })

        // TODO
        //        .def("get_input", &Patch::get_input)

        .def("get_auto_free", &Patch::get_auto_free)
        .def("set_auto_free", &Patch::set_auto_free)
        .def_property("auto_free", &Patch::get_auto_free, &Patch::set_auto_free)

        .def("get_auto_free_node", &Patch::get_auto_free_node)
        .def("set_auto_free_node", &Patch::set_auto_free_node)
        .def_property("auto_free_node", &Patch::get_auto_free_node, &Patch::set_auto_free_node)

        .def("get_trigger_node", &Patch::get_trigger_node)
        .def("set_trigger_node", &Patch::set_trigger_node)
        .def_property("trigger_node", &Patch::get_trigger_node, &Patch::set_trigger_node)

        .def_property("output", &Patch::get_output, &Patch::set_output)
        .def_property_readonly("nodes", &Patch::get_nodes)
        .def_property_readonly("inputs", &Patch::get_inputs)
        .def_property_readonly("graph", &Patch::get_graph)
        .def_property_readonly("state", &Patch::get_state)

        .def("play", [](PatchRef patch) { return patch->get_graph()->play(patch); })
        .def("stop", [](PatchRef patch) { patch->get_graph()->stop(patch); })
        .def("add_to_graph", [](PatchRef patch) {
            try
            {
                patch->get_graph()->add_patch(patch);
                return true;
            }
            catch (const cpu_usage_above_limit_exception &e)
            {
                return false;
            }
        })
        .def("trigger", [](Patch &patch) { patch.trigger(); })
        .def("trigger", [](Patch &patch, std::string name) { patch.trigger(name); })
        .def("trigger", [](Patch &patch, std::string name, float value) { patch.trigger(name, value); })

        /*--------------------------------------------------------------------------------
         * Patch specification methods
         *-------------------------------------------------------------------------------*/
        .def("add_input", [](Patch &patch, std::string name) { return patch.add_input(name); })
        .def("add_input", [](Patch &patch, std::string name, float value) { return patch.add_input(name, value); })
        .def("add_input", [](Patch &patch, std::string name, NodeRef value) { return patch.add_input(name, value); })
        .def("add_buffer_input", [](Patch &patch, std::string name) { return patch.add_buffer_input(name); })
        .def("add_buffer_input", [](Patch &patch, std::string name, BufferRef value) { return patch.add_buffer_input(name, value); })

        .def("add_node", &Patch::add_node)
        .def("set_output", &Patch::set_output)
        .def("to_spec", &Patch::to_spec);

    py::class_<PatchSpec, PatchSpecRefTemplate<PatchSpec>>(m, "PatchSpec")
        .def(py::init<std::string>())
        .def(py::init<>())
        .def_property_readonly("name", &PatchSpec::get_name)
        .def("print", [](PatchSpec &patchspec) { patchspec.print(); })
        .def("load", &PatchSpec::load)
        .def("save", &PatchSpec::save)
        .def("to_json", &PatchSpec::to_json)
        .def("from_json", &PatchSpec::from_json);

    py::class_<PatchRegistry>(m, "PatchRegistry")
        .def(py::init(&PatchRegistry::global))
        .def("create", &PatchRegistry::create);

    /*--------------------------------------------------------------------------------
     * Constants and enums
     *-------------------------------------------------------------------------------*/
    py::enum_<signalflow_patch_state_t>(m, "signalflow_patch_state_t", py::arithmetic(), "signalflow_patch_state_t")
        .value("SIGNALFLOW_PATCH_STATE_ACTIVE", SIGNALFLOW_PATCH_STATE_ACTIVE, "Active")
        .value("SIGNALFLOW_PATCH_STATE_STOPPED", SIGNALFLOW_PATCH_STATE_STOPPED, "Stopped")
        .export_values();
}
