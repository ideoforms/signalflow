title: StereoPanner node documentation
description: StereoPanner: Pans a mono input to a stereo output. `pan` should be between -1 (hard left) to +1 (hard right), with 0 = centre.

[Reference library](../../index.md) > [Processors: Panning](../index.md) > [StereoPanner](index.md)

# StereoPanner

```python
StereoPanner(input=0, pan=0.0)
```

Pans a mono input to a stereo output. `pan` should be between -1 (hard left) to +1 (hard right), with 0 = centre.

### Examples

```python

#-------------------------------------------------------------------------------
# Using StereoPanner to pan a low pitch to the left and a high pitch to the
# right.
#-------------------------------------------------------------------------------
low = TriangleOscillator(220)
high = TriangleOscillator(660)

left = StereoPanner(low, -0.8)
right = StereoPanner(high, 0.8)

output = (left + right) * 0.5
output.play()

```

```python

#-------------------------------------------------------------------------------
# Using StereoPanner to repeatedly pan an arpeggiating oscillator between the 
# left and right channels.
#-------------------------------------------------------------------------------
clock = Impulse(8.0)
CMaj7 = [ 60, 64, 67, 71, 74, 76 ] * 8
FMaj9 = [ 65, 69, 72, 76, 77, 81 ] * 8
arpeggios = CMaj7 + FMaj9
sequence = Sequence(arpeggios, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = TriangleOscillator(frequency)
release = Line(0.1, 0.5, 12, True)
envelope = ASREnvelope(0.0, 0.0, release, 1.0, clock)
voice = oscillator * envelope

pan = SineLFO(0.1667, -1.0, 1.0)
output = StereoPanner(voice, pan)
output.play()

```

