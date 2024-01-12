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
# Impulse generator producing an impulse every second (60bpm)
#-------------------------------------------------------------------------------
impulse = Impulse(frequency=1.0)
output = impulse * 0.5
output.play()```

