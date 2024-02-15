title: SawOscillator node documentation
description: SawOscillator: Produces a (non-band-limited) sawtooth wave, with the given `frequency` and `phase` offset. When a `reset` or trigger is received, resets the phase to zero.

[Reference library](../../index.md) > [Oscillators](../index.md) > [SawOscillator](index.md)

# SawOscillator

```python
SawOscillator(frequency=440, phase=None, reset=None)
```

Produces a (non-band-limited) sawtooth wave, with the given `frequency` and `phase` offset. When a `reset` or trigger is received, resets the phase to zero.

### Examples

```python

#-------------------------------------------------------------------------------
# Simple saw wave oscillator shaped by an envelope
#-------------------------------------------------------------------------------
saw = SawOscillator(440)
envelope = ASREnvelope(0.05, 0.1, 0.5)
output = StereoPanner(saw * envelope) * 0.5
output.play()

```

