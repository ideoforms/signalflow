#include "signal/python/python.h"

void init_python_constants(py::module &m);
void init_python_node(py::module &m);
void init_python_nodes(py::module &m);
void init_python_buffer(py::module &m);
void init_python_graph(py::module &m);
void init_python_patch(py::module &m);
void init_python_exceptions(py::module &m);
void init_python_util(py::module &m);

PYBIND11_MODULE(libsignal, m)
{
    m.doc() = R"pbdoc(
    signalflow
    ----------

    A framework for audio DSP.
    )pbdoc";

    init_python_constants(m);
    init_python_node(m);
    init_python_nodes(m);
    init_python_graph(m);
    init_python_buffer(m);
    init_python_patch(m);
    init_python_exceptions(m);
    init_python_util(m);
}
