title: BufferPlayer node documentation
description: BufferPlayer: Plays the contents of the given buffer. `start_time`/`end_time` are in seconds. When a `clock` signal is received, rewinds to the `start_time`.

[Reference library](../../index.md) > [Buffer](../index.md) > [BufferPlayer](index.md)

# BufferPlayer

```python
BufferPlayer(buffer=None, rate=1.0, loop=0, start_time=None, end_time=None, clock=None)
```

Plays the contents of the given buffer. `start_time`/`end_time` are in seconds. When a `clock` signal is received, rewinds to the `start_time`. 

 Set `clock` to `0` to prevent the buffer from being triggered immediately.

