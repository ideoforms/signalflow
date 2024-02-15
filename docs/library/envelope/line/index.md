title: Line node documentation
description: Line: Line segment with the given start/end values, and duration (in seconds). If loop is true, repeats indefinitely. Retriggers on a clock signal.

[Reference library](../../index.md) > [Envelope](../index.md) > [Line](index.md)

# Line

```python
Line(start=0.0, end=1.0, time=1.0, loop=0, clock=None)
```

Line segment with the given start/end values, and duration (in seconds). If loop is true, repeats indefinitely. Retriggers on a clock signal.

### Examples

```python

#-------------------------------------------------------------------------------
# Using a line to control the gain of an oscillator, emulating a sidechain 
# ducking effect.
#-------------------------------------------------------------------------------
clock = Impulse(frequency=1.0)
line = Line(0.0, 0.5, 0.5, False, clock)
osc = SawOscillator(200)
output = StereoPanner(osc * line)
output.play()

```

```python

#-------------------------------------------------------------------------------
# Using Line to repeatedly alter the release value of an envelope applied to the
# main synth voice, in time with the music.
#-------------------------------------------------------------------------------
clock = Impulse(8.0)
CMaj7 = [ 60, 64, 67, 71, 74, 76 ] * 8
FMaj9 = [ 65, 69, 72, 76, 77, 81 ] * 8
arpeggios = CMaj7 + FMaj9
sequence = Sequence(arpeggios, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = TriangleOscillator(frequency)
release = Line(0.1, 0.5, 6, True)
envelope = ASREnvelope(0.0, 0.0, release, 1.0, clock)
voice = oscillator * envelope

pan = SineLFO(0.1667, -1.0, 1.0)
output = StereoPanner(voice, pan)
output.play()

```

