title: SquareOscillator node documentation
description: SquareOscillator: Produces a pulse wave with the given `frequency` and pulse `width`,  where `width` of `0.5` is a square wave and other values produce a rectangular wave.

[Reference library](../../index.md) > [Oscillators](../index.md) > [SquareOscillator](index.md)

# SquareOscillator

```python
SquareOscillator(frequency=440, width=0.5)
```

Produces a pulse wave with the given `frequency` and pulse `width`,  where `width` of `0.5` is a square wave and other values produce a rectangular wave.

### Examples

```python

#-------------------------------------------------------------------------------
# Simple square wave oscillator shaped by an envelope
#-------------------------------------------------------------------------------
square = SquareOscillator(440)
envelope = ASREnvelope(0, 0.1, 0.5)
output = square * envelope
output.play()
```

