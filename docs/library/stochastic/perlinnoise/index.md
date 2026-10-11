title: PerlinNoise node documentation
description: PerlinNoise: 2D Perlin noise, sampled at the coordinates given by x and y. Successive octaves are summed with doubling frequency, each scaled in amplitude by detail relative to the previous octave. Output is in the range [-1, 1]. The noise field tiles every 256 units. Calling set_seed() regenerates the field.

[Reference library](../../index.md) > [Stochastic](../index.md) > [PerlinNoise](index.md)

# PerlinNoise

```python
PerlinNoise(x=0.0, y=0.0, num_octaves=4, detail=0.5, reset=None)
```

2D Perlin noise, sampled at the coordinates given by x and y. Successive octaves are summed with doubling frequency, each scaled in amplitude by detail relative to the previous octave. Output is in the range [-1, 1]. The noise field tiles every 256 units. Calling set_seed() regenerates the field.

