# Work with SignalFlow

The recommended way to start exploring SignalFlow is by using the free [Visual Studio Code](https://code.visualstudio.com/) as an editor. Visual Studio Code can edit interactive "Jupyter" notebooks, which allow you to run and modify blocks of Python code in real-time, which is a great way to experiment live with audio synthesis.

### 1. Download and install Visual Studio Code

Visual Studio Code is a [free download from visualstudio.com](https://code.visualstudio.com/Download).

### 2. Install the Python and Jupyter extensions

In Visual Studio Code, browse to `Extensions` via the far-left icons (or press `⇧⌘X`), and install the `Python` and `Jupyter` extensions. These are needed to modify Jupyter notebooks in real-time.

### 3. Create a notebook in Visual Studio Code

Create a new text file in Visual Studio Code (`⌘N`), and save it to a new file with an `.ipynb` extension. This tells VS Code that you are creating a Python notebook. You should see the screen layout change to display an empty black text block (in Jupyter parlance, a "cell").

### 4. Start writing code

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

### 5. Start a SignalFlow session

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