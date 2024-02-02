title: Sequence node documentation
description: Sequence: Outputs the elements in `sequence`, incrementing position on each `clock`.

[Reference library](../../index.md) > [Sequencing](../index.md) > [Sequence](index.md)

# Sequence

```python
Sequence(sequence=std::vector<float> ( ), clock=None)
```

Outputs the elements in `sequence`, incrementing position on each `clock`.

### Examples

```python

#-------------------------------------------------------------------------------
# Creating a sequence using the MIDI note values of a C Major scale, starting on
# middle C.
#-------------------------------------------------------------------------------
clock = Impulse(2.0)
sequence = Sequence([ 60, 62, 64, 65, 67, 69, 71, 72 ], clock)
frequency = MidiNoteToFrequency(sequence)
oscillator = TriangleOscillator(frequency)
oscillator.play()

```

