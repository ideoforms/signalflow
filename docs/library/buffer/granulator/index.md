title: Granulator node documentation
description: Granulator: Granulator. Generates a grain from the given buffer each time a clock signal is received, with the given duration/rate/pan parameters. The input buffer can be mono or stereo.

[Reference library](../../index.md) > [Buffer](../index.md) > [Granulator](index.md)

# Granulator

```python
Granulator(buffer=None, clock=0, pos=0, duration=0.1, pan=0.0, rate=1.0, max_grains=2048)
```

Granulator. Generates a grain from the given buffer each time a clock signal is received, with the given duration/rate/pan parameters. The input buffer can be mono or stereo.

