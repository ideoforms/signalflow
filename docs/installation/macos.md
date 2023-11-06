# Installing SignalFlow on macOS

Installing SignalFlow requires some basic terminal usage. Open Terminal via Finder in `Applications > Utilities > Terminal`.

## 1. Install Python

Download and install the latest version of Python (3.12) from [Python.org](https://www.python.org/downloads/).

To check that it has worked, run the below command in Terminal and confirm that it prints a version starting with 3.12:

```
python3 --version
```

## 2. Set up a virtual environment

To ensure that your SignalFlow Python install does not conflict with other Python packages on your system, we strongly recommend setting up a dedicated Python "virtual environment" for SignalFlow, which is a self-contained area of the file system containing all of the Python packages and binaries needed for the system.

To create the virtual environment:

```
python3 -m venv signalflow-env
```

!!! info
    You can replace `signalflow-env` with whatever name you would like to call the virtual environment.
    You can also create this virtual env anywhere on your drive, by changing the directory with `cd some/other/path`.

To activate the virtual environment:

```
source signalflow-env/bin/activate
```

## 3. Install SignalFlow

Installing SignalFlow is done using the `pip` Python package manager. The below line will install the `signalflow` library, plus support for `jupyter` interactive Python notebooks, and register your virtual environment system-wide so that notebooks can access it.

```
pip3 install signalflow jupyter
python3 -m ipykernel install --name signalflow-env
```

That's it!

The installation of SignalFlow includes a command-line tool, `signalflow`, that can be used to test and configure the framework. Check that the installation has succeeded by playing a test tone through your default system audio output:

```
signalflow test
```

## 4. Work with SignalFlow

Now, let's set up an editor to [work with SignalFlow](/installation/working/). 
    
## Examples

[Several example scripts](https://github.com/ideoforms/signalflow/tree/master/examples/python) are included within the repo, covering simple control and modulation, FM synthesis, sample granulation, MIDI control, chaotic functions, etc.
