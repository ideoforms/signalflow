title: OnsetDetector node documentation
description: OnsetDetector: Simple time-domain onset detector: outputs an impulse when an onset is detected in the input.

[Reference library](../../index.md) > [Analysis](../index.md) > [OnsetDetector](index.md)

# OnsetDetector

```python
OnsetDetector(input=0.0, threshold=2.0, min_interval=0.1)
```

Simple time-domain onset detector: outputs an impulse when an onset is detected in the input. 

 Maintains short-time and long-time averages. An onset is registered when the short-time average is threshold x the long-time average. min_interval is the minimum interval between onsets, in seconds.

