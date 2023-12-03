[Reference library](../index.md) > [Processors: Dynamics](index.md)

# Processors: Dynamics

- **[Compressor](compressor/index.md)**: Dynamic range compression, with optional `sidechain` input. When the input amplitude is above `threshold`, compresses the amplitude with the given `ratio`, following the given `attack_time` and `release_time` in seconds.
- **[Gate](gate/index.md)**: Outputs the input value when it is above the given `threshold`, otherwise zero.
- **[Maximiser](maximiser/index.md)**: Gain maximiser.
- **[RMS](rms/index.md)**: Outputs the root-mean-squared value of the input, in buffers equal to the graph's current buffer size.
