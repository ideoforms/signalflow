title: ChannelArray node documentation
description: ChannelArray: Takes an array of inputs and spreads them across multiple channels of output.

[Reference library](../../index.md) > [Operators](../index.md) > [ChannelArray](index.md)

# ChannelArray

```python
ChannelArray()
```

Takes an array of inputs and spreads them across multiple channels of output.

### Examples

```python

#-------------------------------------------------------------------------------
# Using ChannelArray to pan a low tone to the left and a high tone to the right.
#-------------------------------------------------------------------------------
low = TriangleOscillator(220)
high = TriangleOscillator(660)
panned = ChannelArray([low, high]) * 0.3
panned.play()

```

