title: SVFilter node documentation
description: SVFilter: State variable filter. `filter_type` can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. `resonance` should be between `[0..1]`.

[Reference library](../../index.md) > [Processors: Filters](../index.md) > [SVFilter](index.md)

# SVFilter

```python
SVFilter(input=0.0, filter_type=SIGNALFLOW_FILTER_TYPE_LOW_PASS, cutoff=440, resonance=0.0)
```

State variable filter. `filter_type` can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. `resonance` should be between `[0..1]`.

### Examples

```python

#-------------------------------------------------------------------------------
# Using SVFilter as a low-pass filter on white noise.
#-------------------------------------------------------------------------------
noise = WhiteNoise()
filtered = SVFilter(input=noise,
                    filter_type="low_pass", 
                    cutoff=1000, 
                    resonance=0.6)
output = StereoPanner(filtered)
output.play()

```

```python

#-------------------------------------------------------------------------------
# Using SVFilter as a low-pass filter to reduce the harshness of a square wave
# oscillator.
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
filtered = SVFilter(input=voice,
                    filter_type= "low_pass", 
                    cutoff=4000, 
                    resonance=0.3)
delayed = AllpassDelay(filtered, 0.15, 0.8, 0.5)

pan = TriangleLFO(0.1, -1.0, 1.0)
output = StereoPanner(delayed, pan) * 0.3
output.play()

```

