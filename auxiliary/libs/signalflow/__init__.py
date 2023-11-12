"""
This package is used on Windows only, as a container for the .pyd file and
platform-specific DLLs.

On Linux and macOS, the bare .so file is copied.
"""
from .signalflow import *
