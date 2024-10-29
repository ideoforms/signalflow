# Troubleshooting: Installation problems

## General Python installation guidelines

- It is generally best to use a version of Python that is separate from the system Python, using an installer from Python.org or a package manager such as [Homebrew](https://brew.sh/).
- To minimise conflicts with other software, you should also ideally create a Python virtual environment, which is outlined in the [easy installation guide](../installation/easy.md).

## On macOS

Note that macOS 10.15 (Catalina) or above is required.

## On Linux

If you are having audio problems with Linux, try switching to a different audio backend. From the command-line, run:

```
signalflow list-backend-names
```

Then, run `signalflow config`, and add a line to select an alternative backend:

```
[audio]
output_backend_name = "pulseaudio"
```

---