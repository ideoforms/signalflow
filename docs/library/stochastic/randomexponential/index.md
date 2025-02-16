title: RandomExponential node documentation
description: RandomExponential: Generate an random exponential value between min/max. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

[Reference library](../../index.md) > [Stochastic](../index.md) > [RandomExponential](index.md)

# RandomExponential

```python
RandomExponential(min=0.001, max=1.0, clock=None, reset=None)
```

Generate an random exponential value between min/max. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

