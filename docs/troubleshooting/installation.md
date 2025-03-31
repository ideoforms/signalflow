# Troubleshooting: Installation problems

## General Python installation guidelines

- It is generally best to use a version of Python that is separate from the system Python, using an installer from Python.org or a package manager such as [Homebrew](https://brew.sh/).
- To minimise conflicts with other software, you should also ideally create a Python virtual environment, which is outlined in the [easy installation guide](../installation/easy.md).

## On macOS

- macOS 12 (Monterey) or above is required
- Some archive releases are available supporting macOS 10.15 (Catalina and above), but note that these may not contain the latest fixes and features

## On Linux

If you are having audio problems with Linux, try switching to a different audio backend. From the command-line, run:

```
signalflow list-backend-names
```

Then, run `signalflow config`, and add a line to select an alternative backend:

```
[audio]
backend_name = "pulseaudio"
```

## On Raspberry Pi

SignalFlow requires Raspberry Pi 3 Model B or above. Note that only 64-bit devices and operating systems are supported (`uname -m` should display `aarch64`). 

---
