title: ClockDivider node documentation
description: ClockDivider: When given a `clock` input (e.g., an Impulse), divides the clock by the given `factor`. factor must be an integer greater than or equal to 1.

[Reference library](../../index.md) > [Sequencing](../index.md) > [ClockDivider](index.md)

# ClockDivider

```python
ClockDivider(clock=0, factor=1)
```

When given a `clock` input (e.g., an Impulse), divides the clock by the given `factor`. factor must be an integer greater than or equal to 1.

### Examples

```python

#-------------------------------------------------------------------------------
# Using a ClockDivider to create rhythms related to the main clock. Here the 
# oscillator panned left is heard on every tick of the clock. The oscillator 
# panned right is heard every 3 ticks of the clock.
#-------------------------------------------------------------------------------
clock = Impulse(2.0)
divided_clock = ClockDivider(clock, 3)

oscillator_a = TriangleOscillator(220)
oscillator_b = TriangleOscillator(440)

envelope_a = ASREnvelope(0.01, 0.0, 0.25, 1.0, clock)
envelope_b = ASREnvelope(0.01, 0.0, 0.5, 1.0, divided_clock)

voice_a = oscillator_a * envelope_a * 0.5
voice_b = oscillator_b * envelope_b * 0.5

left = StereoPanner(voice_a, -0.75)
right = StereoPanner(voice_b, 0.75)

left.play()
right.play()


```

