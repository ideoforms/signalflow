title: SquareLFO node documentation
description: SquareLFO: Produces a pulse wave LFO with the given `frequency` and pulsewidth of `width`, ranging from `min` to `max`, where `width` of `0.5` is a square wave.

[Reference library](../../index.md) > [Oscillators](../index.md) > [SquareLFO](index.md)

# SquareLFO

```python
SquareLFO(frequency=1.0, min=0.0, max=1.0, width=0.5, phase=0.0)
```

Produces a pulse wave LFO with the given `frequency` and pulsewidth of `width`, ranging from `min` to `max`, where `width` of `0.5` is a square wave.

### Examples

```python

#-------------------------------------------------------------------------------
# Alarm effect, using a pulse wave LFO to modulate a sinewave's frequency
#-------------------------------------------------------------------------------
lfo = SquareLFO(1, 200, 400)
sine = SineOscillator(lfo)
sine.play()
```

