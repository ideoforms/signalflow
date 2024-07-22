# Troubleshooting: InsufficientBufferSizeException

## Example

```
Exception in AudioGraph: Node audioout-soundio cannot render because output
buffer size is insufficient (8192 samples requested, buffer size = 2048).
Increase the buffer size.
```

## Description

This exception occurs because the audio hardware is requesting more samples than a node has currently allocated. This allocation is controlled by SignalFlow's `output_buffer_size` config parameter.   

## Solution

Increase the `output_buffer_size` within your [SignalFlow configuration](../graph/config.md). This can be done by adding the below to your `~/.signalflow/config` file: 

```
[audio]
output_buffer_size = 8192
```
