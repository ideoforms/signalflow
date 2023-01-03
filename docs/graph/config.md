# The AudioGraph

## Graph configuration

There are a number of graph configuration parameters that can be used to change the global behaviour of the audio system. 

| Parameter   | Description                                                                                                                                                                                                                                                                                                                                                                                                            |
|--------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| output_device_name             | The name of the audio output device to use. This must precisely match the device's name in your system. If not found, `DeviceNotFoundException` is thrown when instantiating the graph.                                                                                                                                                                                                                                |
| input_device_name              | The name of the input device to use.                                                                                                                                                                                                                                                                                                                                                                                   |
| sample_rate                    | The audio sample rate to use.                                                                                                                                                                                                                                                                                                                                                                                          |
| output_buffer_size             | The size of the hardware output audio buffer, in samples. A larger buffer reduces the chance of buffer overflows and glitches, but at the cost of higher latency. Note that this config option merely specifies the **preferred** output buffer size, which may not be available in the system hardware. To check the actual buffer size used by the AudioGraph, query `graph.output_buffer_size` after instantiation. |
| input_buffer_size              | The size of the hardware input audio buffer.                                                                                                                                                                                                                                                                                                                                                                           |
| cpu_usage_limit                | Imposes a hard limit on the CPU usage permitted by SignalFlow. If the estimated (single-core) CPU usage exceeds this value, no more nodes or patches can be created until it returns to below the limit. Floating-point value between 0..1, where 0.5 means 50% CPU.                                                                                                                                                   |

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

---

[→ Next: Graph status and properties](properties.md)
