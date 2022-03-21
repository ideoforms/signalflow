# The AudioGraph

!!! warning
    This documentation is a work-in-progress and may have sections that are missing or incomplete.

`AudioGraph` is the global audio processing graph that schedules and performs the audio processing.

It contains references to all of the [Node](/node) and [Patch](/patch) objects that are currently active, and, when a new block of audio is requested by the system audio I/O layer, traverses the tree of nodes and generates new samples.

## Creating the graph

By default, a new `AudioGraph` object connects to the system's default audio hardware device (via the integrated `libsoundio` library), using the system's default sample rate and buffer size. 

## Changing the graph config

### Programmatically

The graph config has to be specified before it is started. To specify an alternative config, create and populate an `AudioGraphConfig` object:

```
config = AudioGraphConfig()
config.output_device_name = "MacBook Pro Speakers"
config.sample_rate = 44100
config.output_buffer_size = 2048

graph = AudioGraph(config)
```

For a full list of available settings, see `pydoc3 signalflow.AudioGraphConfig`.

### Via .signalflow/config

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

Two methods are provided to query the graph's current operations.

### Status

Querying `graph.status` returns a one-line description of the number of nodes and patches in the graph, and the estimated CPU usage:

```
>>> graph.status
AudioGraph: 235 active nodes, 6 patches, 13.953516% CPU usage
```

To automatically poll and print the graph's status periodically, call `graph.poll(interval)`, where `interval` is in seconds:

```
>>> graph.poll(1)
AudioGraph: 118 active nodes, 3 patches, 7.097344% CPU usage
AudioGraph: 118 active nodes, 3 patches, 7.166250% CPU usage
AudioGraph: 40 active nodes, 1 patch, 2.601211% CPU usage
```

!!! note
    Note that `poll` does not print any output in Jupyter notebooks due to the way that Jupyter handles stdout/stderr.

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

## Clearing and stopping the graph

To clear all nodes and patches from the graph but leave it running for further audio synthesis:

```
>>> graph.clear()
```

To stop the graph and tear down audio I/O:

```
>>> graph.stop()
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