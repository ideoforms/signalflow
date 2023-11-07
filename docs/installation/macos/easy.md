# SignalFlow: Easy install for macOS

The simplest way to start exploring SignalFlow is with the free [Visual Studio Code](https://code.visualstudio.com/) editor. Visual Studio Code can edit interactive "Jupyter" notebooks, which allow you to run and modify blocks of Python code in real-time, which is a great way to experiment live with audio synthesis.

You'll only need to do this installation process once. Once setup, experimenting with SignalFlow is as simple as opening Visual Studio Code.  

## 1. Install Python

Download and install the latest version of Python (currently 3.12).

[Download Python](https://www.python.org/downloads/){ .md-button }

## 2. Download and install Visual Studio Code

Download and install the latest version of Visual Studio Code.

[Download Visual Studio Code](https://code.visualstudio.com/Download){ .md-button }

Once installed, open `Applications` and run `Visual Studio Code`.

## 3. Create a new Visual Studio Code workspace

In Visual Studio code, create a new folder to contain your new SignalFlow project:

 - Select `File → Open Folder...`
 - Select `New Folder`, and pick a name for your new project folder

!!! info "Where to put your workspace"
    You can store your project workspace anywhere on your drive. The workspace can hold multiple notebooks, audio files, etc.

!!! warning "Trusted workspaces"
    If Visual Studio asks "Do you trust the authors of the files in this folder?", select "Yes, I trust the authors". This is a security mechanism to protect you against untrusted third-party code.

## 4. Install the Python and Jupyter extensions

Visual Studio Code requires extensions to be installed to handle Python and Jupyter files.

In Visual Studio Code, select the `Extensions` icon from in the far-left column (or press `⇧⌘X`), and install the `Python` and `Jupyter` extensions by searching for their names and clicking "Install" on each.

Once installation has finished, close the `Extensions` tab.

## 5. Create a notebook in Visual Studio Code

Select `File → New File...` (`^⌥⌘N`), and select `Jupyter Notebook`. You should see the screen layout change to display an empty black text block (in Jupyter parlance, a "cell"). 

## 6. Create a Python virtual environment to use

Click the button marked `Select Kernel` in the top right. 

 - Select `Python Environments...`
 - Select `Create Python Environment`
 - Select `Venv`
 - Finally, select the version of Python you just installed (`3.12.x`).

Visual Studio Code will launch into some activity, in which it is installing necessary libraries and creating a Python "virtual environment", which is an isolated area of the filesystem containing all of the packages needed for this working space. Working in different virtual environments for different projects is good practice to minimise the likelihood of conflicts and disruptions.

When the setup is complete, the button in the top right should change to say `.venv (Python 3.12.x)`.

You're now all set to start writing code!

## 7. Start writing code

In the first block, type:

```python
print("Hello, world")
```

To run the cell, press `^↵` (control-enter). You should see "Hello, world" appear below the cell. You're now able to edit, change and run Python code in real-time!

!!! info "Keyboard shortcuts"
    - Navigate between cells with the arrow keys
    - Use `enter` to begin editing a cell, and `escape` to end editing and move to select mode
    - In select mode, use `b` to add a cell after the current cell, and `a` to add a cell before it 
    - To evaluate a cell and move on to the next cell, use `⇧↵` (shift-enter)

## 8. Start a SignalFlow session

Clear the first cell, and replace it with:

```python
from signalflow import *
```

Run the cell with `^↵`. This imports all of the SignalFlow commands and classes.

Create a new cell (`b`), and in the new cell, run:

```python
graph = AudioGraph()
```

This will create and start a new global audio processing system, using the system's default audio output. You should see the name of the audio device printed to the notebook.

In a new cell, run:

```python
sine = SineOscillator(440) * 0.1
sine.play()
```

This will create a sine oscillator, attenuate it, and play it from the system. Hopefully you should now hear a tone playing from your speaker or headphones.

To stop the playback, create a new cell and run:

```python
sine.stop()
```