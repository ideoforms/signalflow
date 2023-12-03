title: Stutter node documentation
description: Stutter: Stutters the input whenever a trigger is received on `clock`. Generates `stutter_count` repeats, with duration of `stutter_time`.

[Reference library](../../index.md) > [Processors: Delays](../index.md) > [Stutter](index.md)

# Stutter

Stutters the input whenever a trigger is received on `clock`. Generates `stutter_count` repeats, with duration of `stutter_time`.

Signature:
```python
Stutter(input=0.0, stutter_time=0.1, stutter_count=1, clock=None, max_stutter_time=1.0)
```
