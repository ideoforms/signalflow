title: WhiteNoise node documentation
description: WhiteNoise: Generates whitenoise between min/max. If frequency is zero, generates at audio rate. For frequencies lower than audio rate, interpolate applies linear interpolation between values, and random_interval specifies whether new random values should be equally-spaced or randomly-spaced.

[Reference library](../../index.md) > [Stochastic](../index.md) > [WhiteNoise](index.md)

# WhiteNoise

```python
WhiteNoise(frequency=0.0, min=-1.0, max=1.0, interpolate=true, random_interval=true, reset=None)
```

Generates whitenoise between min/max. If frequency is zero, generates at audio rate. For frequencies lower than audio rate, interpolate applies linear interpolation between values, and random_interval specifies whether new random values should be equally-spaced or randomly-spaced.

### Examples

```python

#-------------------------------------------------------------------------------
# Using white noise to control the pitch of an oscillator.
# A new pitch is determined once every second. Interpolation is turned off so 
# that the oscillator jumps to the new pitch instead of smoothly moving to it.
# Random interval is turned off so that pitch changes occur at a regular rate.
#-------------------------------------------------------------------------------
frequency = WhiteNoise( frequency=1,
                        min=100, 
                        max=1000, 
                        interpolate=False, 
                        random_interval=False)
oscillator = SineOscillator(frequency)
output = StereoPanner(oscillator) * 0.5
output.play()

```

```python

#-------------------------------------------------------------------------------
# Using white noise to simulate the sound of wind.
# White noise is generated at audio rate and passed into a band-pass filter.
# The cutoff of the filter is controlled by another white noise generator, which
# generates a new value between 100 and 300 at randomly-spaced intervals every 
# second, and smoothly interpolates between these values.
#-------------------------------------------------------------------------------
noise = WhiteNoise()
cutoff = WhiteNoise(1, 100, 300, True, True)
filtered = SVFilter(input=noise,
                    filter_type= "band_pass", 
                    cutoff=cutoff,
                    resonance=0.8)
output = StereoPanner(filtered) * 0.5
output.play()

```

