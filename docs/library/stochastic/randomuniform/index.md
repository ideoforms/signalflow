title: RandomUniform node documentation
description: RandomUniform: Generates a uniformly random value between min/max. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

[Reference library](../../index.md) > [Stochastic](../index.md) > [RandomUniform](index.md)

# RandomUniform

```python
RandomUniform(min=0.0, max=1.0, clock=None, reset=None)
```

Generates a uniformly random value between min/max. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

