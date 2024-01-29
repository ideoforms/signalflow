# Accessing in memory

The floating-point samples within a SignalFlow `Buffer` can be read and written directly from Python.

## Using get and set methods

The `get()` and `set()` methods can be used to access individual samples, indexed by channel and frame offset.

```python
# Create a 2-channel buffer
buf = Buffer(2, 256)

# Set the sample in channel 1 at index 20 to 0.5
buf.set(1, 20, 0.5)

# Confirm that the sample is set correctly
assert buf.get(1, 20) == 0.5
```

## As a numpy array

The `.data` property of a `Buffer` points to a numpy array of shape `(num_channels, num_frames)`, which can be used to read or write the buffer's data in real time.

```python
import time

# Create and play a one-second silent buffer
buf = Buffer(2, graph.sample_rate)
player = BufferPlayer(buf, loop=True)
player.play()

# Gradually add crackles to the buffer, which will be heard in real-time
while True:
    buf.data[0][np.random.randint(0, graph.sample_rate)] = 1.0
    buf.data[1][np.random.randint(0, graph.sample_rate)] = 1.0
    time.sleep(1)
```

## Filling a buffer with the result of a function

Just like when [creating a buffer](creating.md#initialising-a-buffer-with-the-result-of-a-function), an existing buffer can be filled with the output of a Python function.



---

[→ Next: Arithmetic operators](operators.md)