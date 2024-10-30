# SignalFlow: Easy install with Visual Studio Code

The simplest way to start exploring SignalFlow is with the free [Visual Studio Code](https://code.visualstudio.com/) editor. Visual Studio Code can edit interactive Jupyter notebooks, allowing you to run and modify blocks of Python code in real-time, which is a great way to experiment live with audio synthesis.

You'll only need to do this installation process once. Once set up, experimenting with SignalFlow is as simple as opening Visual Studio Code.  

---

## 1. Install Python

Download and install the latest version of Python.

[Download Python](https://www.python.org/downloads/){ .md-button }

---

## 2. Download and install Visual Studio Code

Download and install the latest version of Visual Studio Code.

[Download Visual Studio Code](https://code.visualstudio.com/Download){ .md-button }

Once installed, open Visual Studio Code.

---

## 3. Install the Python and Jupyter extensions

Visual Studio Code requires extensions to be installed to handle Python and Jupyter files.

In Visual Studio Code, select the `Extensions` icon from in the far-left column, and install the `Python` and `Jupyter` extensions by searching for their names and clicking "Install" on each.

Once installation has finished, close the `Extensions` tab.

---

## 4. Create a new workspace

In Visual Studio code, create a new folder to contain your new SignalFlow project:

 - Select `File → Open Folder...`
 - Select `New Folder`, and pick a name for your new project folder

!!! info "Where to put your workspace"
    You can store your project workspace anywhere on your drive. The workspace can hold multiple notebooks, audio files, etc.

!!! warning "Trusted workspaces"
    If Visual Studio asks "Do you trust the authors of the files in this folder?", select "Yes, I trust the authors". This is a security mechanism to protect you against untrusted third-party code.

---

## 5. Create a notebook

Select `File → New File...`, and select `Jupyter Notebook`. You should see the screen layout change to display an empty black text block (in Jupyter terminology, a "cell"). 

---

## 6. Create a Python virtual environment to use

Click the button marked `Select Kernel` in the top right. 

 - Select `Python Environments...`
 - Select `Create Python Environment`
 - Select `Venv`
 - Finally, select the version of Python you just installed.

!!! info "Multiple versions of Python?"
    If you already have one or more versions of Python installed, any version from Python 3.8 upwards is fine.

Visual Studio Code will launch into some activity, in which it is installing necessary libraries and creating a Python "virtual environment", which is an isolated area of the filesystem containing all the packages needed for this working space. Working in different virtual environments for different projects is good practice to minimise the likelihood of conflicts and disruptions.

When the setup is complete, the button in the top right should change to say `.venv (Python 3.x.x)`.

!!! info
    New notebooks created within this workspace will share the same Python virtual environment.  

---

## 7. Install SignalFlow

In the first block, copy and paste the below:

```python
%pip install signalflow
```

To run the cell, press `Ctrl-Enter`. After a minute, you should see some output saying `Successfully installed signalflow`.

!!! info "Running cells with '.venv' requires the ipykernel package."
    If you are given a prompt that the `ipykernel` package is required, press "Install" to install the package.

You're now all set to start writing code!

---

## 8. Start writing code

In a Jupyter interactive notebook, you can write and run multi-line blocks of Python code. Press `enter` to edit the cell, delete its contents, and paste the below.    

```python
print("Hello")
print("world!")
```

Press `Ctrl-Enter` to run the cell. You should see "Hello world!" appear below the cell. 

!!! info "Keyboard shortcuts"
    - Navigate between cells with the arrow keys
    - Press `enter` to begin editing a cell, and `escape` to end editing and move to select mode
    - In select mode, use `b` to add a cell after the current cell, and `a` to add a cell before it 
    - To evaluate a cell and move on to the next cell, use `⇧↵` (shift-enter)

---

## 9. SignalFlow: Import the library and start audio processing 

Clear the first cell, and replace it with:

```python
from signalflow import *
```

Run the cell with `Ctrl-Enter`. This command imports all of the SignalFlow commands and classes, and only needs to be run once per session.

Create a new cell by pressing `b`, and in the new cell, run:

```python
graph = AudioGraph()
```

This will create and start a new global audio [processing graph](../graph/index.md), using the system's default audio output. You should see the name of the audio device printed to the notebook.

This also needs to be run once per session. In fact, only one global `AudioGraph` object can be created.  

---

## 10. SignalFlow: Make some sound

We're finally ready to make some noise!

In a new cell, copy and paste the below:

```python
sine = SineOscillator(440)
panner = StereoPanner(sine, 0.0)
output = panner * 0.1
output.play()
```

This will create a simple sine wave oscillator, pan it over a stereo pair, attenuate it, and play it from the system's audio output. Hopefully you should now hear a tone playing from your speaker or headphones.

One of the benefits of coding interactively is that you can modify the parameters of a synthesis network while it is running. In a new cell, try modifying the `frequency` property of the oscillator:

```python
sine.frequency = 880
```

You should hear it increase in pitch. Try changing the value to something different and re-running the cell.

The `pan` property of `StereoPanner` controls its position in the stereo field from left to right, and can range from `-1` to `1`:

```python
panner.pan = -1
```

Finally, to stop all playback:

```python
graph.clear()
```

---

{%
   include-markdown "installation/next-steps.md"
%}
