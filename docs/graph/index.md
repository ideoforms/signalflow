# The AudioGraph

!!! warning
    This documentation is a work-in-progress and may have sections that are missing or incomplete.

`AudioGraph` is the global audio processing graph that schedules and performs the audio processing.

It contains references to all of the [Node](/node) and [Patch](/patch) objects that are currently active, and, when a new block of audio is requested by the system audio I/O layer, traverses the tree of nodes and generates new samples.

## Creating the graph

Creating the graph is simple: `graph = AudioGraph()`

By default, a new `AudioGraph` immediately connects to the system's default audio hardware device (via the integrated `libsoundio` library), using the system's default sample rate and buffer size.

!!! info
    Note that the AudioGraph is a singleton object: only one AudioGraph can be created, which is shared globally.

To prevent the graph from starting instantly (for example, if you want to use the graph in offline mode), pass `start=False` to the constructor.

To configure graph playback or recording parameters, see the following section.

## Graph configuration

There are a number of graph configuration parameters that can be used to change the global behaviour of the audio system. 

| Parameter   | Description                                                                                                                                                                                                                                                                                                                                                                                                            |
|--------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `output_device_name`                       | The name of the audio output device to use. This must precisely match the device's name in your system.                                                                                                                                                                                                                                                                                                                |
| `input_device_name`                        | The name of the input device to use.                                                                                                                                                                                                                                                                                                                                                                                   |
| `sample_rate`                              | The audio sample rate to use.                                                                                                                                                                                                                                                                                                                                                                                          |
| `output_buffer_size`                       | The size of the hardware output audio buffer, in samples. A larger buffer reduces the chance of buffer overflows and glitches, but at the cost of higher latency. Note that this config option merely specifies the **preferred** output buffer size, which may not be available in the system hardware. To check the actual buffer size used by the AudioGraph, query `graph.output_buffer_size` after instantiation. |
| `input_buffer_size`                        | The size of the hardware input audio buffer.                                                                                                                                                                                                                                                                                                                                                                           |
| `cpu_usage_limit`                          | Imposes a hard limit on the CPU usage permitted by SignalFlow. If the estimated (single-core) CPU usage exceeds this value, no more nodes or patches can be created until it returns to below the limit. Floating-point value between 0..1, where 0.5 means 50% CPU.                                                                                                                                                   |

### Configuring the graph programmatically

To specify an alternative config, create and populate an `AudioGraphConfig` object before the graph is started:

```
config = AudioGraphConfig()
config.output_device_name = "MacBook Pro Speakers"
config.sample_rate = 44100
config.output_buffer_size = 2048

graph = AudioGraph(config)
```

### Configuring the graph via ~/.signalflow/config

To specify a configuration that is used by all future SignalFlow sessions, create a file `~/.signalflow/config` with the following format:

```
[audio]
sample_rate = 48000
output_buffer_size = 256
input_buffer_size = 256
output_device_name = "MacBook Pro Speakers"
input_device_name = "MacBook Pro Microphone"
```

All fields are optional.

## Querying the graph 

A number of methods are provided to query the graph's current operations.

### Status

Querying `graph.status` returns a one-line description of the number of nodes and patches in the graph, and the estimated CPU and RAM usage:

```
>>> graph.status
AudioGraph: 235 active nodes, 6 patches, 13.95% CPU usage, 34.91MB memory usage
```

To automatically poll and print the graph's status periodically, call `graph.poll(interval)`, where `interval` is in seconds:

```
>>> graph.poll(1)
AudioGraph: 118 active nodes, 3 patches, 7.09% CPU usage, 34.91MB memory usage
AudioGraph: 118 active nodes, 3 patches, 7.16% CPU usage, 34.91MB memory usage
AudioGraph: 40 active nodes, 1 patch, 2.60% CPU usage, 34.91MB memory usage
```

To stop polling, call `graph.poll(0)`.

### Structure

Querying `graph.structure` returns a multi-line string describing every Node in the graph, their parameter values, and their connectivity structure.

```
>>> graph.structure
 * audioout-soundio
   input0:
    * linear-panner
      pan: 0.000000
      input:
       * multiply
         input1: 0.251189
         input0:
          * sine
            frequency: 440.000000
```

### Other graph properties

- `graph.node_count` (int): Returns the current number of [Nodes](/node) in the graph (including within patches)
- `graph.patch_count` (int): Returns the current number of [Patches](/patch) in the graph
- `cpu_usage` (float): Returns the current CPU usage, between 0.0 (0%) and 1.0 (100%). CPU usage can be lowered by increasing the output buffer size.
- `memory_usage` (int): Returns the current RAM usage, in bytes. This is typically mostly used by waveform data in [Buffers](/buffer).
- `num_output_channels` (int): Returns the graph's current output channel count, which is typically identical to the number of channels supported by the audio output device.
- `output_buffer_size` (int): Returns the current hardware output buffer size, in bytes.

## Clearing and stopping the graph

To clear all nodes and patches from the graph but leave it running for further audio synthesis:

```
>>> graph.clear()
```

To stop the graph and pause audio I/O:

```
>>> graph.stop()
```

To permanently destroy the graph:

```
>>> graph.destroy()
```

## Recording the audio output of the graph

Convenience methods are provided to make it easy to record the global audio output:

```python
graph.start_recording("filename.wav")
...
graph.stop_recording()
```

To record output in formats other than the default stereo, `start_recording` takes a `num_channels` argument that can be used to specify an alternative channel count.

!!! note
    At present, only .wav is supported as an output format for global audio recordings. 