title: Envelope node documentation
description: Envelope: Generic envelope constructor, given arrays of levels, times and curves that describe the shape of the envelope over time.

[Reference library](../../index.md) > [Envelope](../index.md) > [Envelope](index.md)

# Envelope

```python
Envelope(levels=std::vector<NodeRef> ( ), times=std::vector<NodeRef> ( ), curves=std::vector<NodeRef> ( ), clock=None, loop=false)
```

Generic envelope constructor, given arrays of levels, times and curves that describe the shape of the envelope over time. 

 Note that there must be one fewer time than levels, and the curves array should match the number of segments between levels.

### Examples

```python

# Looping envelope different curve shapes
sine = SineOscillator(880)
envelope = Envelope(levels=[0.0, 1.0, 0.0], times=[0.5, 0.5], curves=[2, 0.5], loop=True)
output = sine * envelope
output.play()


```

```python

# Percussive envelope with sharp attack and gradual decay, triggered by an impulse
noise = WhiteNoise()
impulse = Impulse(0.5)
envelope = Envelope(levels=[0.0, 1.0, 0.1, 0.0], times=[0.01, 0.1, 1.0], clock=impulse)
output = noise * envelope
output.play()


```

