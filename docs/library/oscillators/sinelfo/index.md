title: SineLFO node documentation
description: SineLFO: Produces a sinusoidal LFO at the given `frequency` and `phase` offset, with output ranging from `min` to `max`.

[Reference library](../../index.md) > [Oscillators](../index.md) > [SineLFO](index.md)

# SineLFO

```python
SineLFO(frequency=1.0, min=0.0, max=1.0, phase=0.0)
```

Produces a sinusoidal LFO at the given `frequency` and `phase` offset, with output ranging from `min` to `max`.

### Examples

```python

# Siren effect, using a sinewave LFO to modulate a sawtooth's frequency
lfo = SineLFO(1, 200, 1000)
saw = SawOscillator(lfo)
saw.play()
```

