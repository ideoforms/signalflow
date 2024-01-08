title: TriangleLFO node documentation
description: TriangleLFO: Produces a triangle LFO with the given `frequency`, ranging from `min` Hz to `max` Hz, with the given `phase` offset.

[Reference library](../../index.md) > [Oscillators](../index.md) > [TriangleLFO](index.md)

# TriangleLFO

```python
TriangleLFO(frequency=1.0, min=0.0, max=1.0, phase=0.0)
```

Produces a triangle LFO with the given `frequency`, ranging from `min` Hz to `max` Hz, with the given `phase` offset.

### Examples

```python

#-----------------------------------------------------------------------------------
# Pacman ghost sound, using a triangle wave LFO to modulate a sine wave's frequency
#-----------------------------------------------------------------------------------
lfo = TriangleLFO(3, 200, 900)
sine = SineOscillator(lfo)
sine.play()
```