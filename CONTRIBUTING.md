# Contributing

- [Building from source](#building-from-source)
- [Authoring a new Node](#authoring-a-new-node)
- [Test](#test)
- [Release](#release)
- [Documentation](#documentation)

## Building from source

Building from source assumes that you have a working installation of Python 3, ideally within a virtual environment.

<details>
<summary>Building from source: <b>macOS</b></summary>

### macOS

To build on macOS from source, install dependencies with Homebrew:
```
brew install cmake libsndfile libsoundio
```

Clone this repository, then build and install with `pip`:
```
pip3 install .
```
</details>

<details>
<summary>Building from source: <b>Linux, Raspberry Pi</b></summary>

### Linux, Raspberry Pi

SignalFlow supports Linux (verified on Ubuntu 20.04 and Raspberry Pi OS buster) with alsa and pulseaudio backends.

To build the Python library from source on Linux, install dependencies with apt:
```
apt-get install -y git cmake g++ python3-pip libasound2-dev libsndfile1-dev libsoundio-dev fftw3-dev
```

If you experience an error on Raspberry Pi `libf77blas.so.3: cannot open shared object file`:

```
sudo apt-get install -y libatlas-base-dev
```

Clone this repository, then build and install with `pip`:
```
pip3 install .
```

</details>

<details>
<summary>Building from source: <b>Windows</b></summary>

### Windows

This is work in progress.

Currently, dependencies need to be downloaded and built by hand. These can be placed anywhere.

- https://github.com/timmb/libsoundio - check out the `fix-msvc` branch.
  - Use CMake GUI to build libsoundio with Visual Studio 2019 with binaries in a subfolder of that repo named `build`. (Configure, Generate, Open project, Batch build all configurations)
- https://github.com/libsndfile/libsndfile
  - Use CMake GUI to build libsndfile with Visual Studio 2019 with binaries in a subfolder of that repo named `build`. (Configure, Generate, Open project, Batch build all configurations)
- Download Windows binaries of FFTW from http://fftw.org/install/windows.html.

To build SignalFlow, use the CMake GUI. Press configure and you will see three empty fields to fill in with the path to the two build folders and the FFTW binaries folder (see above). Set these parameters then press Configure, then Generate then Open. Then build in Visual Studio 2019.

As of 2021-03-03, only the signalflow project has been ported to build correctly on Windows. Only tested in x64 and for Debug builds. Tested using Visual Studio 2019.

</details>

<details>
<summary>Building from source: <b>Debug symbols</b></summary>

To build the Python libraries with debug symbols:
```
python3 setup.py build --debug install
```
</details>

<details>
<summary>Building from source: <b>C++ only</b> (no Python layer)</summary>

To build and install the C++ core without the Python binding layer:
```
mkdir build
cd build
cmake ..
make -j8
```

To build with debug symbols, include `-DCMAKE_BUILD_TYPE=Debug` when calling `cmake`.

</details>


## Authoring a new Node

To author a new Node class:

- Create the .h header file in the appropriate subdirectory of `source/include/signalflow/node`
- Create the .cpp source file in the appropriate subdirectory of `source/src/node`
- Add the header file to `source/include/signalflow/signalflow.h`
- Add the source file to `source/src/CMakeLists.txt`
- Regenerate the auto-generated bindings and docs: `auxiliary/scripts/auto-generator.py --library --readme --bindings`
- Add unit tests to the appropriate test script in `tests`
- Re-run the tests: `python3 setup.py test`
- Update the stubs (see [Generating stubs](#generating-stubs))

Note that, to comply with the registry system, all arguments to Node constructors must have default values so that the Node class can be instantiated with a zero-argument syntax, e.g. `NodeClass()`.

## Error handling

Exceptions should not be raised from the `process()` function, as this function runs in the audio thread and exceptions will not be correctly caught by the Python interpreter, meaning that, particularly in environments such as Jupyter, the process will appear to crash.

Instead, the `signalflow_audio_thread_error()` function should be called. This outputs the exception to `stderr`, and halts the graph process. The exception string should start with the name of the Node class, to make it clear where the error originates, ideally with a hint on how to resolve the issue. For example:

```
CombDelay: Delay time exceeds maximum. Reduce the delay_time, or increase max_delay_time.
```

## Test

To run the unit test suite:
```
python3 setup.py test
```

## Release

For each new release:

- Update the version in `setup.py`
- Add entry to `CHANGELOG.md`
- Commit changes
- Add a git version tag, in format `v1.2.34`
- Add a GitHub release with title `v1.2.34`, using the latest `CHANGELOG` entry as the release body 
- Build new wheels:
  - Linux x86: wheels are automatically built using GitHub actions
  - Linux rpi: wheels must be built manually using `auxiliary/cibuildwheel/make-raspberry-pi-aarch64.sh`
  - macOS: wheels must be built manually using `auxiliary/cibuildwheel/make-macos-x86-arm64.sh`
  - Windows: wheels must be built manually (TBC)
- Upload wheels to pypi: `twine upload wheelhouse/1.2.34/*`

## Documentation

To update autogenerated Node documentation:

```
auxiliary/scripts/auto-generator.py --library --readme
```

To add examples to the node library, simply add a Python script inside the corresponding Node class's directory inside `docs/library`. Any `.py` script found within a node class directory will be automatically included in the markdown documentation by `auto-generator.py`. By convention, examples are normally named `example-0.py`, `example-1.py`, etc.

To generate and render the HTML documentation, install `mkdocs` and associated plugins:

```
# use specific mkdocs-material version for full "Last update" text in git revision-date
pip3 install mkdocs mkdocs-material==9.1.15 mkdocs-include-markdown-plugin mkdocs-git-revision-date-localized-plugin
```

To serve the HTML documentation locally, on `http://localhost:8000`:
```
mkdocs serve
```

To deploy docs to GitHub:
```
mkdocs gh-deploy
```

### Generating stubs

To generate [stubs](https://peps.python.org/pep-0561/) for IDE type hinting and inline documentation, the [pybind11-stubgen](https://github.com/sizmailov/pybind11-stubgen) package is required. Once installed, run:

```
pybind11-stubgen -o auxiliary/libs/signalflow-stubs signalflow
```

The `signalflow-stubs` package is bundled with the standard SignalFlow wheel, and as per [PEP 561's convention for stub-only packages](https://peps.python.org/pep-0561/#stub-only-packages), should be used by IDEs to display type hints and docs. This has been verified in Visual Studio Code 1.84.1.
