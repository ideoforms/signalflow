title: YinPitchTracker node documentation
description: YinPitchTracker: YIN pitch tracker

[Reference library](../../index.md) > [Analysis](../index.md) > [YinPitchTracker](index.md)

# YinPitchTracker

```python
YinPitchTracker(input=0.0, threshold=0.1, f0_min=50.0, f0_max=2000.0, window_size=1024)
```

YIN pitch tracker 

 Implements the YIN fundamental frequency estimator algorithm (de Cheveigné & Kawahara, 2002) 

 Tracks pitch on channel 0 (monophonic) and outputs estimated f0 on all channels.

