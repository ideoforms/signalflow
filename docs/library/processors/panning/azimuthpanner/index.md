title: AzimuthPanner node documentation
description: AzimuthPanner: Pan input around an equally-spaced ring of `num_channels` speakers. `pan` is the pan position from -1..+1, where 0 = centre front. `width` is the source's width, where 1.0 spans exactly between an adjacent pair of channels.

[Reference library](../../index.md) > [Processors: Panning](../index.md) > [AzimuthPanner](index.md)

# AzimuthPanner

Pan input around an equally-spaced ring of `num_channels` speakers. `pan` is the pan position from -1..+1, where 0 = centre front. `width` is the source's width, where 1.0 spans exactly between an adjacent pair of channels.

Signature:
```python
AzimuthPanner(num_channels=2, input=0, pan=0.0, width=1.0)
```
