# The AudioGraph

## Graph configuration

There are a number of graph configuration parameters that can be used to change the global behaviour of the audio
system. This can be
done [programmatically](#configuring-the-graph-programmatically), [via a config file](#configuring-the-graph-via-signalflowconfig),
or [via environmental variables](#configuring-the-graph-via-environmental-variables).

| Parameter           | Description                                                                                                                                                                                                                                                                                                                                                                                                            |
|---------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| backend_name | The name of the audio backend to use, which can be one of: `jack`, `alsa`, `pulseaudio`, `coreaudio`, `wasapi`, `dummy`. Defaults to the first of these found on the system. Typically only required for Linux.                                                                                                                                                                                                 |
| output_device_name  | The name of the audio output device to use. Note  If not found, `DeviceNotFoundException` is thrown when instantiating the graph.                                                                                                                                                                                                                                                                                      |
| output_buffer_size  | The size of the hardware output audio buffer, in samples. A larger buffer reduces the chance of buffer overflows and glitches, but at the cost of higher latency. Note that this config option merely specifies the **preferred** output buffer size, which may not be available in the system hardware. To check the actual buffer size used by the AudioGraph, query `graph.output_buffer_size` after instantiation. |
| input_device_name   | The name of the input device to use.                                                                                                                                                                                                                                                                                                                                                                                   |
| input_buffer_size   | The size of the hardware input audio buffer.                                                                                                                                                                                                                                                                                                                                                                           |
| sample_rate         | The audio sample rate to use.                                                                                                                                                                                                                                                                                                                                                                                          |
| cpu_usage_limit     | Imposes a hard limit on the CPU usage permitted by SignalFlow. If the estimated (single-core) CPU usage exceeds this value, no more nodes or patches can be created until it returns to below the limit. Floating-point value between 0..1, where 0.5 means 50% CPU.                                                                                                                                                   |
| auto_record         | If true, automatically records the graph's audio output to a timestamped file in `~/.signalflow/recordings`. A true value can be any of "true", "yes", "on", or 1.                                                                                                                                                                                                                                                     |

---

## Configuring the graph programmatically

To specify an alternative config, create and populate an `AudioGraphConfig` object before the graph is started:

```
config = AudioGraphConfig()
config.output_device_name = "MacBook Pro Speakers"
config.sample_rate = 44100
config.output_buffer_size = 2048

graph = AudioGraph(config)
```

---

## Configuring the graph via ~/.signalflow/config

To specify a configuration that is used by all future SignalFlow sessions, create a file `~/.signalflow/config`,
containing one or more of the "Graph configuration" fields listed above.

For example:

```
[audio]
sample_rate = 48000
output_buffer_size = 256
input_buffer_size = 256
output_device_name = "MacBook Pro Speakers"
input_device_name = "MacBook Pro Microphone"
```

All fields are optional.

A quick and easy way to edit your config, or create a new config file, is by using the `signalflow` command-line
utility:

```
signalflow configure
```

This will use your default `$EDITOR` to open the configuration, or `pico` if no editor is specified.

---

## Configuring the graph via environmental variables

SignalFlow config can also be set by setting an environmental variable in your shell. Variable names are identical to
the upper-case version of the config string, prefixed with `SIGNALFLOW_`. For example:

```
export SIGNALFLOW_OUTPUT_DEVICE_NAME="MacBook Pro Speakers"
export SIGNALFLOW_OUTPUT_BUFFER_SIZE=1024
export SIGNALFLOW_AUTO_RECORD=true
```

---

## Printing the current config

To print the current configuration to stdout:

```
graph.config.print()
```

---

[→ Next: Graph status and properties](properties.md)
