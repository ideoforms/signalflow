[Reference library](../index.md) > [Buffer](index.md)

# Buffer

- **[BeatCutter](beatcutter.md)**: Cuts a buffer into segment_count segments, and stutters/jumps with the given probabilities.
- **[BufferLooper](bufferlooper.md)**: Read and write from a buffer concurrently, with controllable overdub.
- **[BufferPlayer](bufferplayer.md)**: Plays the contents of the given buffer. start_time/end_time are in seconds. When a clock signal is receives, rewinds to the start_time.
- **[BufferRecorder](bufferrecorder.md)**: Records the input to a buffer. feedback controls overdub.
- **[FeedbackBufferReader](feedbackbufferreader.md)**: Counterpart to FeedbackBufferWriter.
- **[FeedbackBufferWriter](feedbackbufferwriter.md)**: Counterpart to FeedbackBufferReader.
- **[GrainSegments](grainsegments.md)**: GrainSegments
- **[Granulator](granulator.md)**: Granulator. Generates a grain from the given buffer each time a clock signal is received, with the given duration/rate/pan parameters. The input buffer can be mono or stereo.
- **[SegmentPlayer](segmentplayer.md)**: Trigger segments of a buffer at the given onset positions.
