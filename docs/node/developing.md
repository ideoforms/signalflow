# Nodes

## Developing new Node classes

Node classes can be developed in Python, which is useful for prototyping DSP algorithms, or in C++, for efficient real-time processing.

### Developing a Node class in Python

To define a new Node class in Python, create a subclass of `Node`, and implement its `process()` method. Once defined, it can be used just like any other Node: it can be played, connected to the inputs of other nodes, combined with [operators](operators.md), and used within a Patch.

```python
from signalflow import *
import numpy as np

class OnePole(Node):
    def __init__(self, input=0, coefficient=0.5):
        super().__init__()
        self.create_input("input", input)
        self.create_input("coefficient", coefficient)
        self.history = np.zeros(32)

    def process(self, num_frames):
        for channel in range(self.num_output_channels):
            input = self.input.output_buffer[channel]
            coefficient = self.coefficient.output_buffer[channel]
            output = self.output_buffer[channel]
            value = self.history[channel]
            for frame in range(num_frames):
                value = (1 - coefficient[frame]) * input[frame] + coefficient[frame] * value
                output[frame] = value
            self.history[channel] = value

graph = AudioGraph()
filtered = OnePole(WhiteNoise(), 0.95)
output = filtered * 0.5
output.play()
graph.wait()
```

- **Constructor**: In the constructor, be sure to call `super().__init__()` before calling any other `Node` methods.
- **Inputs**: To create an audio-rate input, call  `self.create_input(name, value)`. This can then be read and set like any other node's input (e.g. `filtered.coefficient = 0.99`). Inside `process()`, read an input's samples from `self.<name>.output_buffer`, a 2D numpy array of shape `(channels, frames)`.
- **process(num_frames)**: The central processing method, which is called once per block. In this method, you must write `self.num_output_channels` x `num_frames` samples into `self.output_buffer`.
- **Channels**: By default, a node outputs the same number of channels as its widest input. To specify that the Node always generates a fixed number of channels, call `self.set_channels(num_input_channels, num_output_channels)` in the constructor.
- **State**: State can be stored in normal Python attributes (e.g. `self.history`).

#### Performance and real-time considerations

Python nodes are well-suited to offline rendering (e.g. `graph.render_to_new_buffer()`).

However, note that Python is much slower than C++ for per-sample processing. Where possible, operate on whole blocks with vectorised numpy operations. For recursive per-sample algorithms such as filters, consider moving the inner loop into a function compiled with [numba](https://numba.pydata.org/)'s `@njit`.

The [python-node-example.py](https://github.com/ideoforms/signalflow/blob/master/examples/python-node-example.py) example implements the one-pole filter above in three ways, and benchmarks each. Recursive filters can't be vectorised with numpy alone, as each output sample depends on the previous one, so the block-based version uses `scipy.signal.lfilter`, which only supports a fixed coefficient per block. Typical results for a mono filter at 44.1kHz with 256-sample blocks (Apple M4 Pro), against a time budget of 5805µs per block:

| Implementation | Time per block | Speed relative to real time |
|----------------|----------------|-----------------------------|
| Pure Python    | 220µs          | 26x                         |
| SciPy `lfilter`| 6.6µs          | 880x                        |
| Numba `@njit`  | 3.1µs          | 1900x                       |

All three produce identical output. Around 2µs of each block is the fixed cost of calling into Python and accessing `output_buffer` arrays.

!!! warning
    During real-time playback, `process()` is called from the audio thread, which must acquire Python's Global Interpreter Lock (GIL) for each block. This works well while the main thread is idle in `graph.wait()`, but if your script is busy running other Python code, the audio thread may have to wait for the GIL, causing glitches or dropouts.

If an exception is raised within `process()` during real-time playback, the error is printed and the graph is stopped. During offline rendering, the exception is raised to the caller as normal.

### Developing a Node class in C++

See [CONTRIBUTING.md](https://github.com/ideoforms/signalflow/blob/master/CONTRIBUTING.md)
