title: TriangleOscillator node documentation
description: TriangleOscillator: Produces a triangle wave with the given `frequency`.

[Reference library](../../index.md) > [Oscillators](../index.md) > [TriangleOscillator](index.md)

# TriangleOscillator

```python
TriangleOscillator(frequency=440)
```

Produces a triangle wave with the given `frequency`.

### Examples

```python

#-------------------------------------------------------------------------------
# Simple triangle wave oscillator shaped by an envelope
#-------------------------------------------------------------------------------
tri = TriangleOscillator(440)
envelope = ASREnvelope(0.1, 0.1, 0.5)
output = tri * envelope
output.play()
```

