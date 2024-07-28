[Reference library](../index.md) > [Buffer](index.md)

# Buffer

- **[BeatCutter](beatcutter/index.md)**: Cuts a buffer into segment_count segments, and stutters/jumps with the given probabilities.
- **[BufferLooper](bufferlooper/index.md)**: Read and write from a buffer concurrently, with controllable overdub.
- **[BufferPlayer](bufferplayer/index.md)**: Plays the contents of the given buffer. `start_time`/`end_time` are in seconds. When a `clock` signal is received, rewinds to the `start_time`.  Set `clock` to `0` to prevent the buffer from being triggered immediately.
- **[BufferRecorder](bufferrecorder/index.md)**: Records the input to a buffer. feedback controls overdub.
- **[FeedbackBufferReader](feedbackbufferreader/index.md)**: Counterpart to FeedbackBufferWriter.
- **[FeedbackBufferWriter](feedbackbufferwriter/index.md)**: Counterpart to FeedbackBufferReader.
- **[SegmentPlayer](segmentplayer/index.md)**: Trigger segments of `buffer` at the given list of `onsets` positions, in seconds. `index` determines the index of the onset to play back at, which can also be passed as an argument to trigger(). `rate` determines the playback rate, and `clock` can be used to retrigger based on the output of another Node. If `continue_after_segment` is non-zero, playback will continue after the subsequent onset.
