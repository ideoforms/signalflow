# Creating a Buffer

A Buffer can be created from:

- [a sound file](#loading-a-buffer-from-a-sound-file)
- [an array of samples](#creating-a-buffer-from-an-array-of-samples)
- [a specified dimension](#creating-an-empty-buffer)
- [the result of a function](#initialising-a-buffer-with-the-result-of-a-function)

## Loading a buffer from a sound file

To load an audio buffer from a sound file, pass the file path to Buffer's constructor.

```python
# Load and play a buffer
buf = Buffer("filename.wav")
player = BufferPlayer(buf)
player.play()
```

The type of the audio file is automatically inferred from the type and contents. Supported formats include `wav`, `aif`, `mp3`, `ogg`, `flac`, and many other audio formats. 

Interally, file I/O is handled by `libsndfile`. For a full list of supported files, see the [libsndfile documentation](http://www.mega-nerd.com/libsndfile/).

---

## Creating a buffer from an array of samples

To create and initialise a buffer from an existing array of samples, pass the array to Buffer's constructor. Both native Python arrays and `numpy` arrays are supported.

Note that audio samples should always range between `-1.0` and `1.0` to avoid distortion. 

```python
# Initialise a buffer from a native 1D array containing a sawtooth wave
samples = [(n % 100) / 100 - 0.5 for n in range(44100)]
buf = Buffer(samples)
player = BufferPlayer(buf)
player.play()
```

If the array is 1D, a mono buffer will be created. If the array is 2D, a multichannel buffer will be created.

```python
# Initialise a buffer from a numpy 2D array containing a stereo sine wave
import numpy as np

t = np.linspace(0, 1, 44100)
stereo = np.array([np.sin(220 * t * np.pi * 2),
                   np.sin(225 * t * np.pi * 2)])
buf = Buffer(stereo * 0.1)
player = BufferPlayer(buf)
player.play()
```

---

## Creating an empty buffer

An empty buffer can be initialised by specifying its dimensions. All samples will be initialised to zero.

```python
# Create an empty buffer with 2 channels containing 44100 samples each.
buf = Buffer(2, 44100)
```

---

## Initialising a buffer with the result of a function

A buffer can also be populated with the result of a Python function, which takes a single argument containing the index of the frame to be filled.

```python
# Create a buffer containing a 440Hz ping
import numpy as np
buf = Buffer(1, graph.sample_rate,
             lambda frame: np.sin(frame * 440 * np.pi * 2 / graph.sample_rate) * (1 - frame / graph.sample_rate))
player = BufferPlayer(buf)
player.play()
```

---

[→ Next: Saving and exporting a buffer](exporting.md)