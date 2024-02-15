title: CombDelay node documentation
description: CombDelay: Comb delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.

[Reference library](../../index.md) > [Processors: Delays](../index.md) > [CombDelay](index.md)

# CombDelay

```python
CombDelay(input=0.0, delay_time=0.1, feedback=0.5, max_delay_time=0.5)
```

Comb delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.

### Examples

```python

#-------------------------------------------------------------------------------
# Using CombDelay to change the character of a saw wave oscillator.
#-------------------------------------------------------------------------------
clock = Impulse(4)
arpeggio = [60, 62, 64, 66, 68, 70,
            72, 70, 68, 66, 64, 62]
sequence = Sequence(arpeggio, clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = SawOscillator(frequency)
envelope = ASREnvelope(0.1, 0, 0.2, 1.0, clock)
voice = oscillator * envelope
comb = CombDelay(input=voice, 
                 delay_time=0.09, 
                 feedback=0.6, 
                 max_delay_time=0.9)

output = StereoPanner(comb) * 0.5
output.play()

```

