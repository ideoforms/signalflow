title: SquareLFO node documentation
description: SquareLFO: Produces a pulse wave LFO at the given `frequency`, ranging from `min` Hz to `max` Hz, with a pulsewidth of `width` (where `width` of `0.5` is a square wave), with the given `phase` offset.

[Reference library](../../index.md) > [Oscillators](../index.md) > [SquareLFO](index.md)

# SquareLFO

```python
SquareLFO(frequency=1.0, min=0.0, max=1.0, width=0.5, phase=0.0)
```

Produces a pulse wave LFO at the given `frequency`, ranging from `min` Hz to `max` Hz, with a pulsewidth of `width` (where `width` of `0.5` is a square wave), with the given `phase` offset.

### Examples

```python

#-------------------------------------------------------------------------------
# Alarm effect, using a square wave LFO to modulate a sinewave's frequency
#-------------------------------------------------------------------------------
lfo = SquareLFO(1, 200, 400)
sine = SineOscillator(lfo)
sine.play()
```
