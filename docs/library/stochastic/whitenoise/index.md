title: WhiteNoise node documentation
description: WhiteNoise: Generates whitenoise between min/max. If frequency is zero, generates at audio rate. For frequencies lower than audio rate, interpolate applies linear interpolation between values, and random_interval specifies whether new random values should be equally-spaced or randomly-spaced.

[Reference library](../../index.md) > [Stochastic](../index.md) > [WhiteNoise](index.md)

# WhiteNoise

Generates whitenoise between min/max. If frequency is zero, generates at audio rate. For frequencies lower than audio rate, interpolate applies linear interpolation between values, and random_interval specifies whether new random values should be equally-spaced or randomly-spaced.

Signature:
```python
WhiteNoise(frequency=0.0, min=-1.0, max=1.0, interpolate=true, random_interval=true, reset=None)
```
