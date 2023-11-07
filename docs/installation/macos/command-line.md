# SignalFlow: Command-line installation for macOS

These instructions assume you have a working version of Python 3.8+, installed either via [Homebrew](http://brew.sh) or from [Python.org](https://www.python.org/downloads/).

## 1. Set up a virtual environment

We strongly recommend setting up a dedicated Python "virtual environment" for SignalFlow

```
python3 -m venv signalflow-env
source signalflow-env/bin/activate
```

## 2. Install SignalFlow

Installing SignalFlow with `pip`:

```shell
pip3 install signalflow jupyter
python3 -m ipykernel install --name signalflow-env
```

If the installation succeeds, you should see `Successfully installed signalflow`.

## 3. Line test

The installation of SignalFlow includes a command-line tool, `signalflow`, that can be used to test and configure the framework. Check that the installation has succeeded by playing a test tone through your default system audio output.

This may take a few seconds to run for the first time. To exit the test, press ctrl-C (`^C`).

```
signalflow test
```

## Examples

[Several example scripts](https://github.com/ideoforms/signalflow/tree/master/examples) are included within the repo, covering simple control and modulation, FM synthesis, sample granulation, MIDI control, chaotic functions, etc.
