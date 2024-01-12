title: SineOscillator node documentation
description: SineOscillator: Produces a sine wave at the given `frequency`.

[Reference library](../../index.md) > [Oscillators](../index.md) > [SineOscillator](index.md)

# SineOscillator

```python
SineOscillator(frequency=440)
```

Produces a sine wave at the given `frequency`.

### Examples

```python
#-------------------------------------------------------------------------------
# Simple sine wave oscillator shaped by an envelope
#-------------------------------------------------------------------------------
sine = SineOscillator(440)
envelope = ASREnvelope(0.1, 0.1, 0.5)
output = sine * envelope
output.play()
```