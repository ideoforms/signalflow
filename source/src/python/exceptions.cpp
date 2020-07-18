#include "signalflow/python/python.h"

void init_python_exceptions(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Exceptions
     *-------------------------------------------------------------------------------*/
    py::register_exception<signalflow::graph_not_created_exception>(m, "GraphNotCreatedException");
    py::register_exception<signalflow::invalid_channel_count_exception>(m, "InvalidChannelCountException");
}
