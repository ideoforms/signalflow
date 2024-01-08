title: SawLFO node documentation
description: SawLFO: Produces a sawtooth LFO at the given `frequency`, with output ranging from `min` Hz to `max` Hz, with the given `phase` offset.

[Reference library](../../index.md) > [Oscillators](../index.md) > [SawLFO](index.md)

# SawLFO

```python
SawLFO(frequency=1.0, min=0.0, max=1.0, phase=0.0)
```

Produces a sawtooth LFO at the given `frequency`, with output ranging from `min` Hz to `max` Hz, with the given `phase` offset.

### Examples

```python

#-------------------------------------------------------------------------------
# Siren effect, using a sawtooth LFO to modulate a sinewave's frequency
#-------------------------------------------------------------------------------
lfo = SawLFO(1, 200, 1000)
sine = SineOscillator(lfo)
sine.play()
```