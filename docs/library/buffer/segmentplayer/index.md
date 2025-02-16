title: SegmentPlayer node documentation
description: SegmentPlayer: Trigger segments of `buffer` at the given list of `onsets` positions, in seconds.

[Reference library](../../index.md) > [Buffer](../index.md) > [SegmentPlayer](index.md)

# SegmentPlayer

```python
SegmentPlayer(buffer=None, onsets={}, index=None, rate=1.0, start_offset=None, clock=None, continue_after_segment=0)
```

Trigger segments of `buffer` at the given list of `onsets` positions, in seconds. 

 `index` determines the index of the onset to play back at, which can also be passed as an argument to trigger(). `rate` determines the playback rate, and `clock` can be used to retrigger based on the output of another Node. If `continue_after_segment` is non-zero, playback will continue after the subsequent onset.

