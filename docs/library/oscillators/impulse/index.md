title: Impulse node documentation
description: Impulse: Produces a value of 1 at the given `frequency`, with output of 0 at all other times. If frequency is 0, produces a single impulse.

[Reference library](../../index.md) > [Oscillators](../index.md) > [Impulse](index.md)

# Impulse

```python
Impulse(frequency=1.0)
```

Produces a value of 1 at the given `frequency`, with output of 0 at all other times. If frequency is 0, produces a single impulse.

### Examples

```python

#-------------------------------------------------------------------------------
# Using an Impulse node as a clock to trigger an envelope once per second.
#-------------------------------------------------------------------------------
clock = Impulse(1.0)
osc = TriangleOscillator(250)
envelope = ASREnvelope(0.01, 0.0, 0.5, 1.0, clock)
output = osc * envelope
output.play()

```

