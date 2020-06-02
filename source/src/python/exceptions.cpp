#include "signal/python/python.h"

void init_python_exceptions(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Exceptions
     *-------------------------------------------------------------------------------*/
    py::register_exception<libsignal::graph_not_created_exception>(m, "GraphNotCreatedException");
}
