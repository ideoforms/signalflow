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
brew install cmake libsndfile
curl https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h -o source/include/signalflow/node/io/output/miniaudio-library.h
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
# If on Raspberry Pi: libfftw3-dev
apt-get install -y git cmake g++ python3-pip libasound2-dev libsndfile1-dev fftw3-dev
curl https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h -o source/include/signalflow/node/io/output/miniaudio-library.h
```

Clone this repository, then build and install with `pip`:
```
pip3 install .
```

</details>

<details>
<summary>Building from source: <b>Windows</b></summary>

### Windows

The build process for SignalFlow on 64-bit Windows has been verified with Visual Studio 2022 and CMake.

- Download Windows binaries of [FFTW](http://fftw.org/install/windows.html) and [libsndfile](https://github.com/libsndfile/libsndfile/releases/), and unzip them in the same filesystem location as the `signalflow` source directory
- Install Python 3, and dependencies: `python -m pip install build delvewheel`
- Build the binary wheel: `python -m build --wheel`
- Copy the libsndfile and fftw binaries into `dlls`
- Bundle the DLL dependencies with the wheel: `python -m delvewheel repair --add-path=dlls *.whl`

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
- Re-run the tests: `python3 -m pytest`
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
python3 -m pytest
```

## Release

For each new release:

- Update the version in `setup.cfg`
- Add entry to `CHANGELOG.md`
- Commit changes
- Add a git version tag, in format `v1.2.34`
- Add a GitHub release with title `v1.2.34`, using the latest `CHANGELOG` entry as the release body 
- Build new wheels:
  - Linux x86: wheels are automatically built using GitHub actions
  - Linux rpi: wheels must be built manually using `auxiliary/cibuildwheel/make-raspberry-pi-aarch64.sh`
  - macOS: wheels must be built manually using `auxiliary/cibuildwheel/make-macos-x86-arm64.sh`
  - Windows: wheels must be built manually: `python -m cibuildwheel`, and renamed from `win32` to `win_amd64`
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
