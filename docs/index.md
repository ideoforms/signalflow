# SignalFlow

!!! warning
    This documentation is a work-in-progress and may have sections that are missing or incomplete.

SignalFlow is an audio synthesis framework whose goal is to make it quick and intuitive to explore complex sonic ideas. It has a simple and consistent Python API, allowing for rapid prototyping in Jupyter, PyCharm, or on the command-line. It comes with over 100 of built-in node classes for creative exploration.

Its core is implemented in C++11, with cross-platform hardware acceleration.  

SignalFlow has robust support for macOS and Linux (including Raspberry Pi), and has work-in-progress support for Windows. The overall project is currently in alpha status, and interfaces may change without warning.

This documentation currently focuses specifically on Python interfaces and examples.

## Overview

At its core, SignalFlow has a handful of key concepts.

- At the top level is the **[AudioGraph](graph)**, which connects to the system's audio input/output hardware.
- The graph comprises of a network of **[Nodes](node)**, each of which performs a single function (for example, generating a cyclical waveform, or filtering an input node). Nodes are connected by input and output relationships: the output of one node may be used to control the frequency of another. As the output of the first node increases, the frequency of the second node increases correspondingly. This modulation is applied on a sample-by-sample basis: all modulation in SignalFlow happens at audio rate.
- Nodes may have multiple **[inputs](node/inputs)**, which determine which synthesis properties can be modulated at runtime.
- A node can also have **[Buffer](buffer)** properties, which contain audio waveform data that can be read and written to, for playback or recording of samples. 
- Nodes can be grouped in a **[Patch](patch)**, which is a user-defined configuration of nodes. A patch may have one or more named [inputs](patch/inputs) that are defined by the user when creating the patch. Patches can be thought of like voices of a synthesizer. A patch can also be set to [automatically remove itself](patch/auto-free) from the graph when a specified node's playback is complete, which is important for automatic memory management.

## Example

Let's take a look at a minimal SignalFlow example. Here, we create and immediately start the `AudioGraph`, construct a stereo sine oscillator, connect the oscillator to the graph's output, and run the graph indefinitely.

```python
from signalflow import *

graph = AudioGraph()
sine = SineOscillator([440, 880])
envelope = ASREnvelope(0.1, 0.1, 0.5)
output = sine * envelope
output.play()
graph.wait()
```

This demo shows a few syntactical benefits that SignalFlow provides to make it easy to work with audio:

 - The 2-item array of frequency values passed to `SineOscillator` is expanded to create a stereo, 2-channel output. If you passed a 10-item array, the output would have 10 channels. ([Read more: Multichannel nodes](node/multichannel))
 - Mathematical operators like `*` can be used to multiply, add, subtract or divide the output of nodes, and creates a new output Node that corresponds to the output of the operation. This example uses an envelope to modulate the amplitude of an oscillator. ([Read more: Node operators](node/operators))
 - Even through the envelope is mono and the oscillator is stereo, SignalFlow does the right thing and upmixes the envelope's values to create a stereo output, so that the same envelope shape is applied to the L and R channels of the oscillator, before creating a stereo output. This is called "automatic upmixing", and is handy when working with multichannel graphs. ([Read more: Automatic upmixing](node/multichannel#automatic-upmixing))

In subsequent examples, we will skip the `import` line and assume you have already imported everything from the `signalflow` namespace.

!!! info
    If you want to keep your namespaces better separated, you might want to do something like the below.
    ```python
    import signalflow as sf

    graph = sf.AudioGraph()
    sine = sf.SineOscillator(440)
    ...
    ```

## Documentation

- [Installation](getting-started/)
- [Example code](http://github.com/ideoforms/signalflow/tree/master/examples/python)
