#pragma once

#include "signal/signal.h"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace libsignal;

namespace py = pybind11;

/*------------------------------------------------------------------------------
 * Exposes the _a literal which is equivalent to py::arg
 * https://pybind11.readthedocs.io/en/stable/basics.html
 *----------------------------------------------------------------------------*/
using namespace pybind11::literals;

/*------------------------------------------------------------------------------
 * Explicitly specify false to avoid warnings on gcc/linux
 *----------------------------------------------------------------------------*/
PYBIND11_DECLARE_HOLDER_TYPE(T, NodeRefTemplate<T>, false)
PYBIND11_DECLARE_HOLDER_TYPE(T, BufferRefTemplate<T>, false)
PYBIND11_DECLARE_HOLDER_TYPE(T, PatchRefTemplate<T>, false)
PYBIND11_DECLARE_HOLDER_TYPE(T, PatchSpecRefTemplate<T>, false)