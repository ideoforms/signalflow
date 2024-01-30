title: Line node documentation
description: Line: Line segment with the given start/end values and duration. If loop is true, repeats indefinitely. Retriggers on a clock signal.

[Reference library](../../index.md) > [Envelope](../index.md) > [Line](index.md)

# Line

```python
Line(start=0.0, end=1.0, time=1.0, loop=0, clock=None)
```

Line segment with the given start/end values and duration. If loop is true, repeats indefinitely. Retriggers on a clock signal.

### Examples

```python

#-------------------------------------------------------------------------------
# Using a line to control the gain of an oscillator, emulating a sidechain ducking effect.
#-------------------------------------------------------------------------------
clock = Impulse(frequency=1.0)
line = Line(0.0, 0.5, 0.5, False, clock)
osc = SawOscillator(200)
output = osc * line
output.play()

```

