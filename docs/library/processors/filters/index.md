[Reference library](../index.md) > [Processors: Filters](index.md)

# Processors: Filters

- **[BiquadFilter](biquadfilter/index.md)**: Biquad filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. Not recommended for real-time modulation; for this, use SVFilter.
- **[DCFilter](dcfilter/index.md)**: Remove low-frequency and DC content from a signal.
- **[EQ](eq/index.md)**: Three-band EQ.
- **[MoogVCF](moogvcf/index.md)**: Moog ladder low-pass filter.
- **[SVFilter](svfilter/index.md)**: State variable filter. `filter_type` can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. `resonance` should be between `[0..1]`.
