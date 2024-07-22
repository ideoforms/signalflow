[Reference library](../index.md) > [Buffer: Granulation](index.md)

# Buffer: Granulation

- **[SegmentedGranulator](segmentedgranulator/index.md)**: Segmented Granulator.
- **[Granulator](granulator/index.md)**: Granulator. Generates a grain from the given buffer each time a trigger is received on the `clock` input. Each new grain uses the given `duration`, `amplitude`, `pan` and `rate` values presented at each input at the moment the grain is created. The input buffer can be mono or stereo. If `wrap` is true, grain playback can wrap around the end/start of the buffer.
