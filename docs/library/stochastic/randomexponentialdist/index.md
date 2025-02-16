title: RandomExponentialDist node documentation
description: RandomExponentialDist: Generate an random value following the exponential distribution. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

[Reference library](../../index.md) > [Stochastic](../index.md) > [RandomExponentialDist](index.md)

# RandomExponentialDist

```python
RandomExponentialDist(scale=0.0, clock=None, reset=None)
```

Generate an random value following the exponential distribution. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

