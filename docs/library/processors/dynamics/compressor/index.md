title: Compressor node documentation
description: Compressor: Dynamic range compression, with optional `sidechain` input. When the input level (a peak envelope, following the given `attack_time` and `release_time` in seconds) is above `threshold`, reduces the gain so that the level above the threshold is divided by `ratio` (in dB).

[Reference library](../../index.md) > [Processors: Dynamics](../index.md) > [Compressor](index.md)

# Compressor

```python
Compressor(input=0.0, threshold=0.1, ratio=2, attack_time=0.01, release_time=0.1, sidechain=None)
```

Dynamic range compression, with optional `sidechain` input. When the input level (a peak envelope, following the given `attack_time` and `release_time` in seconds) is above `threshold`, reduces the gain so that the level above the threshold is divided by `ratio` (in dB).

