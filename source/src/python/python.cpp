#include "signalflow/python/python.h"

void init_python_constants(py::module &m);
void init_python_node(py::module &m);
void init_python_nodes(py::module &m);
void init_python_buffer(py::module &m);
void init_python_config(py::module &m);
void init_python_graph(py::module &m);
void init_python_patch(py::module &m);
void init_python_exceptions(py::module &m);
void init_python_util(py::module &m);

PYBIND11_MODULE(signalflow, m)
{
    m.doc() = R"pbdoc(
    SignalFlow
    ----------

    A framework for audio DSP.
    )pbdoc";

/*--------------------------------------------------------------------------------
 * SIGNALFLOW_VERSION should be defined by setup.py and passed into the build.
 *-------------------------------------------------------------------------------*/
#ifdef SIGNALFLOW_VERSION
    m.attr("__version__") = SIGNALFLOW_VERSION;
#endif

    init_python_constants(m);
    init_python_node(m);
    init_python_nodes(m);
    init_python_config(m);
    init_python_graph(m);
    init_python_buffer(m);
    init_python_patch(m);
    init_python_exceptions(m);
    init_python_util(m);
}
