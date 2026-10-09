[Reference library](../index.md) > [Processors: Dynamics](index.md)

# Processors: Dynamics

- **[Compressor](compressor/index.md)**: Dynamic range compression, with optional `sidechain` input. When the input level (a peak envelope, following the given `attack_time` and `release_time` in seconds) is above `threshold`, reduces the gain so that the level above the threshold is divided by `ratio` (in dB).
- **[Gate](gate/index.md)**: Outputs the input value when it is above the given `threshold`, otherwise zero.
- **[Maximiser](maximiser/index.md)**: Gain maximiser.
- **[RMS](rms/index.md)**: Outputs the root-mean-squared value of the input, in buffers equal to the graph's current buffer size.
