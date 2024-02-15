title: AllpassDelay node documentation
description: AllpassDelay: All-pass delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.

[Reference library](../../index.md) > [Processors: Delays](../index.md) > [AllpassDelay](index.md)

# AllpassDelay

```python
AllpassDelay(input=0.0, delay_time=0.1, feedback=0.5, max_delay_time=0.5)
```

All-pass delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.

### Examples

```python

#-------------------------------------------------------------------------------
# Using AllpassDelay to add a delay effect to a simple melodic sequence.
# The original oscillator can be heard in the left channel.
# The delay effect can be heard in the right channel.
#-------------------------------------------------------------------------------
clock = Impulse(1.0)
sequence = Sequence([ 60, 62, 64, 65, 67, 69, 71, 72 ], clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = TriangleOscillator(frequency)
envelope = ASREnvelope(0, 0.2, 0.3, 1.0, clock)
voice = oscillator * envelope
delayed = AllpassDelay(input=voice, 
                       delay_time=0.4, 
                       feedback=0.8)

output = ChannelArray([ voice, delayed ]) * 0.75
output.play()

```

```python

#-------------------------------------------------------------------------------
# Using AllpassDelay to add a dreamy atmosphere to synth arpeggios
#-------------------------------------------------------------------------------
clock = Impulse(3.5)
Am7 = [ 67, 64, 60, 57 ] * 4
D7 = [ 62, 66, 69, 72] * 4
arpeggios = Am7 + D7
sequence = Sequence(arpeggios, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = SquareOscillator(frequency)
envelope = ASREnvelope(0.1, 0, 0.2, 1.0, clock)
voice = oscillator * envelope
filtered = SVFilter(voice, "low_pass", 4000, 0.3)
delayed = AllpassDelay(input=filtered, 
                       delay_time=0.15, 
                       feedback=0.8)

pan = TriangleLFO(0.1, -1.0, 1.0)
output = StereoPanner(delayed, pan) * 0.5
output.play()

```

