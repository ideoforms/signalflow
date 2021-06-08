# SignalFlow

!!! warning
    This documentation is a work-in-progress and may have sections that are missing or incomplete.

SignalFlow is an audio DSP framework whose goal is to make it quick and intuitive to explore complex sonic ideas. Its core is C++11, with a full Python API, allowing for rapid prototyping (in Jupyter, PyCharm, or python CLI), efficient processing (with cross-platform hardware acceleration), and lots of built-in node classes for creative exploration. 

SignalFlow has robust support for macOS and Linux (including Raspberry Pi), and has work-in-progress support for Windows. The overall project is currently in alpha status, and interfaces may change without warning.

This documentation currently focuses specifically on Python interfaces and examples.

## Overview

At its core, SignalFlow has a handful of key concepts. It comprises a **graph** (or network) of **nodes**, each of which performs a single function (for example, generating a cyclical waveform, or filtering an input node). Nodes are connected by input and output relationships: the output of one node may be used to control the frequency of another. As the output of the first node increases, the frequency of the second node increases correspondingly. This modulation is applied on a sample-by-sample basis: all modulation in SignalFlow happens at audio rate.

Nodes may have multiple **inputs**, which determine the synthesis properties that can be modulated at runtime. A node can also have **buffer** properties, which contain audio waveform data that can be read and written to, for playback or recording of samples.

Nodes can be grouped in a **patch**, which is a user-defined configuration of nodes. A patch may have one or more named inputs that are defined by the user when creating the patch. Patches can be thought of like voices of a synthesizer. A patch can also be set to automatically remove itself from the graph when a specified node's playback is complete, which is useful for automatic memory management.

## Example

Let's take a look at the simplest possible SignalFlow example. Here, we create and immediately start the `AudioGraph`, construct a single-channel sine oscillator with a frequency of 440Hz, connect the oscillator to the graph's output, and run the graph indefinitely.

```python
from signalflow import *

graph = AudioGraph(start=True)
sine = SineOscillator([440, 880])
envelope = EnvelopeASR(0.01, 0.1, 0.5)
output = sine * env
graph.play(output)
graph.wait()
```

This demo shows a few syntactical benefits that SignalFlow provides to make it easy to work with audio:

 - The 2-item array of frequency values passed to `SineOscillator` is expanded to create a 2-channel output. If you passed a 10-item array, the output would have 10 channels. 
 - Mathematical operators like `*` can be used to multiply, add, subtract or divide the output of nodes, and creates a new output Node that corresponds to the output of the operation. This example multiplies an envelope with an oscillator.
 - Even through the envelope is mono and the oscillator is stereo, SignalFlow does the right thing and upmixes the envelope's values to create a stereo output, so that the same envelope shape is applied to the L and R channels of the oscillator, before creating a stereo output. This is called "automatic upmixing", and is very handy when working with multichannel graphs.

In subsequent examples, we will skip the `import` line and assume you have already imported everything from the `signalflow` namespace.

!!! info
    If you want to keep your namespaces better separated, you might want to do something like the below.
    ```python
    import signalflow as sf

    graph = sf.AudioGraph(start=True)
    sine = sf.SineOscillator(440)
    ...
    ```

## Documentation

- [Getting started](getting-started.md)
- [Example code](http://github.com/ideoforms/signalflow/tree/master/examples/python)
