title: Wavetable node documentation
description: Wavetable: Plays the wavetable stored in buffer at the given `frequency` and `phase` offset. `sync` can be used to provide a hard sync input, which resets the wavetable's phase at each zero-crossing.

[Reference library](../../index.md) > [Oscillators](../index.md) > [Wavetable](index.md)

# Wavetable

Plays the wavetable stored in buffer at the given `frequency` and `phase` offset. `sync` can be used to provide a hard sync input, which resets the wavetable's phase at each zero-crossing.

Signature:
```python
Wavetable(buffer=None, frequency=440, phase=0, sync=0, phase_map=None)
```
