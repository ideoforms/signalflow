# Getting started

## Requirements

SignalFlow supports macOS, Linux (including Raspberry Pi), and has alpha support for Windows. 

Python 3.8 or above is required. On macOS, we recommend installing an up-to-date version of Python3 [using Homebrew](https://docs.python-guide.org/starting/install3/osx/): `brew install python3`.

## Installation

On macOS and Linux x86_64, SignalFlow can be installed using `pip`:

```
pip3 install signalflow 
```

Verify that the installation has worked correctly by using the `signalflow` command-line tool to play a test tone through your default system audio output:

```
signalflow test
```

For more detailed installation information, including Windows install and compilation from source, see the [README](https://github.com/ideoforms/signalflow).

## Examples

[Several example scripts](https://github.com/ideoforms/signalflow/tree/master/examples) are included within the repo, covering simple control and modulation, FM synthesis, sample granulation, MIDI control, chaotic functions, etc.
