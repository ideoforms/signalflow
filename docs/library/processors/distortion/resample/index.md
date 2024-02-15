title: Resample node documentation
description: Resample: Resampler and bit crusher. `sample_rate` is in Hz, `bit_rate` is an integer between 0 and 16.

[Reference library](../../index.md) > [Processors: Distortion](../index.md) > [Resample](index.md)

# Resample

```python
Resample(input=0, sample_rate=44100, bit_rate=16)
```

Resampler and bit crusher. `sample_rate` is in Hz, `bit_rate` is an integer between 0 and 16.

### Examples

```python

#-------------------------------------------------------------------------------
# Using Resample to distort a sine wave.
#-------------------------------------------------------------------------------
sine = SineOscillator(440)
crushed = Resample(sine, 11025, 4)
output = StereoPanner(crushed) * 0.3
output.play()

```

