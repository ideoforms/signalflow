title: OneTapDelay node documentation
description: OneTapDelay: Single-tap delay line. `delay_time` must be less than or equal to `max_delay_time`.

[Reference library](../../index.md) > [Processors: Delays](../index.md) > [OneTapDelay](index.md)

# OneTapDelay

```python
OneTapDelay(input=0.0, delay_time=0.1, max_delay_time=0.5)
```

Single-tap delay line. `delay_time` must be less than or equal to `max_delay_time`.

### Examples

```python

#-------------------------------------------------------------------------------
# Using OneTapDelay to create a delay effect with no feedback.
# The original sound is heard in the left channel, and the delayed sound in the
# right channel.
#-------------------------------------------------------------------------------
clock = Impulse(1)
oscillator = TriangleOscillator(440)
envelope = ASREnvelope(0.001, 0, 0.3, 1.0, clock)
voice = oscillator * envelope
delayed = OneTapDelay(voice, 0.25) * 0.5
output = ChannelArray([voice, delayed]) * 0.5
output.play()

```

```python

#-------------------------------------------------------------------------------
# Using OneTapDelay to bring controlled rhythmic interest to a melodic sequence
#-------------------------------------------------------------------------------
clock = Impulse(3.5)
Dm = [ 62, 65, 69 ] * 2
Bdim = [ 59, 62, 65 ] * 2
Gm = [55, 58, 62 ] * 2
Bb = [77, 74, 70 ]
A = [ 76, 73, 69 ]

arpeggios = Dm + Bdim + Gm + Bb + A
sequence = Sequence(arpeggios, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = SquareOscillator(frequency)
envelope = ASREnvelope(0.1, 0, 0.2, 1.0, clock)
voice = oscillator * envelope
filtered = SVFilter(voice, "low_pass", 4000, 0.3)
delayed = filtered + OneTapDelay(filtered, 0.4) * 0.5

output = StereoPanner(delayed) * 0.3
output.play()

```

