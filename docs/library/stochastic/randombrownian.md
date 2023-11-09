[Reference library](../index.md) > [Stochastic](index.md) > [RandomBrownian](randombrownian.md)

# RandomBrownian

Outputs Brownian noise between min/max, with a mean change of delta between samples. If a clock is passed, only generates a new value on a clock tick.

Signature:
```python
RandomBrownian(min=-1.0, max=1.0, delta=0.01, clock=None, reset=None)
```
