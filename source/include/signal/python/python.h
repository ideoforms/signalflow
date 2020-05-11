#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "signal/signal.h"

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
PYBIND11_DECLARE_HOLDER_TYPE(T, SynthRefTemplate<T>, false)
PYBIND11_DECLARE_HOLDER_TYPE(T, SynthSpecRefTemplate<T>, false)