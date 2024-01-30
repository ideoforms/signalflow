title: Granulator node documentation
description: Granulator: Granulator. Generates a grain from the given buffer each time a clock signal is received, with the given duration/rate/pan parameters. The input buffer can be mono or stereo. If `wrap` is true, grain playback can wrap around the end/start of the buffer.

[Reference library](../../index.md) > [Buffer: Granulation](../index.md) > [Granulator](index.md)

# Granulator

```python
Granulator(buffer=None, clock=0, pos=0, duration=0.1, pan=0.0, rate=1.0, max_grains=2048, wrap=false)
```

Granulator. Generates a grain from the given buffer each time a clock signal is received, with the given duration/rate/pan parameters. The input buffer can be mono or stereo. If `wrap` is true, grain playback can wrap around the end/start of the buffer.

