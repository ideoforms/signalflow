# The AudioGraph

## Status and properties 

A number of methods are provided to query the graph's current status and properties.

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

- `graph.node_count` (int): Returns the current number of [Nodes](../node/index.md) in the graph (including within patches)
- `graph.patch_count` (int): Returns the current number of [Patches](../patch/index.md) in the graph
- `cpu_usage` (float): Returns the current CPU usage, between 0.0 (0%) and 1.0 (100%). CPU usage can be lowered by increasing the output buffer size.
- `memory_usage` (int): Returns the current RAM usage, in bytes. This is typically mostly used by waveform data in [Buffers](../buffer/index.md).
- `num_output_channels` (int): Returns the graph's current output channel count, which is typically identical to the number of channels supported by the audio output device.
- `output_buffer_size` (int): Returns the current hardware output buffer size, in bytes.

---

[→ Next: Recording graph output](recording.md)
