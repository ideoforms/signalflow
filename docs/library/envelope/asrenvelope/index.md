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

```python

#-------------------------------------------------------------------------------
# Using ASREnvelope to shape the sound of an oscillator over time.
# The Line node generates a continuously-changing value which we use as the 
# release time.
#-------------------------------------------------------------------------------
clock = Impulse(8.0)
CMaj7 = [ 60, 64, 67, 71, 74, 76 ] * 8
FMaj9 = [ 65, 69, 72, 76, 77, 81 ] * 8
arpeggios = CMaj7 + FMaj9
sequence = Sequence(arpeggios, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = TriangleOscillator(frequency)
release = Line(0.1, 0.5, 6, True)
envelope = ASREnvelope(attack=0.0, 
                       sustain=0.0, 
                       release=release, 
                       curve=1.0, 
                       clock=clock)
voice = oscillator * envelope

pan = SineLFO(0.1667, -1.0, 1.0)
output = StereoPanner(voice, pan)
output.play()

```

