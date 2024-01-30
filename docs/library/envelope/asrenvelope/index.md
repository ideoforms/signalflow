title: ASREnvelope node documentation
description: ASREnvelope: Attack-sustain-release envelope.

[Reference library](../../index.md) > [Envelope](../index.md) > [ASREnvelope](index.md)

# ASREnvelope

```python
ASREnvelope(attack=0.1, sustain=0.5, release=0.1, curve=1.0, clock=None)
```

Attack-sustain-release envelope.

### Examples

```python

#-------------------------------------------------------------------------------
# Using an ASR Envelope to shape a square wave oscillator
#-------------------------------------------------------------------------------
osc = SquareOscillator(500)
envelope = ASREnvelope(0.1, 0.0, 0.5)
output = osc * envelope
output.play()

```

