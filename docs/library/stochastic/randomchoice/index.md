title: RandomChoice node documentation
description: RandomChoice: Pick a random value from the given array. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

[Reference library](../../index.md) > [Stochastic](../index.md) > [RandomChoice](index.md)

# RandomChoice

```python
RandomChoice(values=std::vector<float> ( ), clock=None, reset=None)
```

Pick a random value from the given array. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

