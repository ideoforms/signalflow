title: RandomCoin node documentation
description: RandomCoin: Flip a coin with the given probability. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

[Reference library](../../index.md) > [Stochastic](../index.md) > [RandomCoin](index.md)

# RandomCoin

```python
RandomCoin(probability=0.5, clock=None, reset=None)
```

Flip a coin with the given probability. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

