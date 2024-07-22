# Troubleshooting: Installation problems

## General Python installation guidelines

- You should a version of Python that is separate from the system Python, either using Homebrew or Python.org installers
- You should also ideally create a virtual environment to avoid conflicts with other local Python installs

## On macOS

Please follow the [installation guidelines for macOS](../installation/macos/index.md). Some general recommendations:

 - macOS 10.15 (Catalina) or above is required
 - Python 3.8 or above is required

## On Linux

Unfortunately, Pipewire is not currently supported due to lack of support from the [libsoundio](https://github.com/andrewrk/libsoundio) subsystem that SignalFlow relies on for cross-platform audio I/O. 
 
## On Windows

If `pip install signalflow` returns `ERROR: No matching distribution found for signalflow`, please note that SignalFlow for Windows currently only supports Python 3.12.

