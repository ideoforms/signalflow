title: StereoWidth node documentation
description: StereoWidth: Reduces the width of a stereo signal. When `width` = 1, input is unchanged. When `width` = 0, outputs a pair of identical channels both containing L+R.

[Reference library](../../index.md) > [Processors: Panning](../index.md) > [StereoWidth](index.md)

# StereoWidth

```python
StereoWidth(input=0, width=1)
```

Reduces the width of a stereo signal. When `width` = 1, input is unchanged. When `width` = 0, outputs a pair of identical channels both containing L+R.

### Examples

```python

#-------------------------------------------------------------------------------
# Using StereoWidth to continuously alter the width of a stereo signal.
#-------------------------------------------------------------------------------
low = TriangleOscillator(220)
high = TriangleOscillator(660)
panned = ChannelArray([low, high])
width = StereoWidth(panned, TriangleLFO(0.5, 0, 1)) * 0.3
width.play()

```

