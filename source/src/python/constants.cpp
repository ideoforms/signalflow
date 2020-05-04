#include "signal/python/python.h"

void init_python_constants(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Constants
     *-------------------------------------------------------------------------------*/
    py::enum_<signal_interpolation_mode_t>(m, "signal_interpolation_mode_t", py::arithmetic(), "signal_interpolation_mode_t")
        .value("SIGNAL_INTERPOLATION_NONE", SIGNAL_INTERPOLATION_NONE, "No interpolation")
        .value("SIGNAL_INTERPOLATION_LINEAR", SIGNAL_INTERPOLATION_LINEAR, "Linear interpolation")
        .export_values();
}
