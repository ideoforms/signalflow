---

## 1. Set up a virtual environment

Creating a new virtual environment for SignalFlow minimises the chances of conflict with other local Python installs.

```
python3 -m venv signalflow-env
source signalflow-env/bin/activate
```

---

## 2. Install SignalFlow

Installing SignalFlow with `pip`:

```shell
pip3 install signalflow
```

If the installation succeeds, you should see `Successfully installed signalflow`.

---

## 3. Line test

The installation of SignalFlow includes a command-line tool, `signalflow`, that can be used to test and configure the framework. Check that the installation has succeeded by playing a test tone through your default system audio output:

This may take a few seconds to run for the first time. To exit the test, press Ctrl-C (`^C`).

```
signalflow test
```

---

## 4. Hello, world

In your text editor, create a new `.py` file containing the below code:

```python
from signalflow import *

graph = AudioGraph()
sine = SineOscillator([440, 880])
envelope = ASREnvelope(0.1, 0.1, 0.5)
output = sine * envelope * 0.1
output.play()
graph.wait()
```

When you run the script, you should hear a short stereo "ping".

---

## 5. (Optional) Interactive notebooks in Jupyter

A nice way to experiment with SignalFlow is by using [Jupyter](https://jupyter.org/) interactive notebooks.

Install Jupyter and register this virtual environment as a Jupyter kernel: 

```shell
pip3 install jupyter
python3 -m ipykernel install --name signalflow-env
```

Open a Jupyter notebook:

```
jupyter notebook
```

