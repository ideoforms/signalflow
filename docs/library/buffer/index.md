[Reference library](../index.md) > [Buffer](index.md)

# Buffer

- **[BeatCutter](beatcutter/index.md)**: Cuts a buffer into segment_count segments, and stutters/jumps with the given probabilities.
- **[BufferLooper](bufferlooper/index.md)**: Read and write from a buffer concurrently, with controllable overdub.
- **[BufferPlayer](bufferplayer/index.md)**: Plays the contents of the given buffer. start_time/end_time are in seconds. When a clock signal is receives, rewinds to the start_time.
- **[BufferRecorder](bufferrecorder/index.md)**: Records the input to a buffer. feedback controls overdub.
- **[FeedbackBufferReader](feedbackbufferreader/index.md)**: Counterpart to FeedbackBufferWriter.
- **[FeedbackBufferWriter](feedbackbufferwriter/index.md)**: Counterpart to FeedbackBufferReader.
- **[Granulator](granulator/index.md)**: Granulator. Generates a grain from the given buffer each time a clock signal is received, with the given duration/rate/pan parameters. The input buffer can be mono or stereo.
