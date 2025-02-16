title: RandomBrownian node documentation
description: RandomBrownian: Outputs Brownian noise between min/max, with a mean change of delta between samples. If a clock is passed, only generates a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

[Reference library](../../index.md) > [Stochastic](../index.md) > [RandomBrownian](index.md)

# RandomBrownian

```python
RandomBrownian(min=-1.0, max=1.0, delta=0.01, clock=None, reset=None)
```

Outputs Brownian noise between min/max, with a mean change of delta between samples. If a clock is passed, only generates a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.

