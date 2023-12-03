title: BufferPlayer node documentation
description: BufferPlayer: Plays the contents of the given buffer. start_time/end_time are in seconds. When a clock signal is receives, rewinds to the start_time.

[Reference library](../../index.md) > [Buffer](../index.md) > [BufferPlayer](index.md)

# BufferPlayer

Plays the contents of the given buffer. start_time/end_time are in seconds. When a clock signal is receives, rewinds to the start_time.

Signature:
```python
BufferPlayer(buffer=None, rate=1.0, loop=0, start_time=None, end_time=None, clock=None)
```
