title: BiquadFilter node documentation
description: BiquadFilter: Biquad filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. Not recommended for real-time modulation; for this, use SVFilter.

[Reference library](../../index.md) > [Processors: Filters](../index.md) > [BiquadFilter](index.md)

# BiquadFilter

```python
BiquadFilter(input=0.0, filter_type=SIGNALFLOW_FILTER_TYPE_LOW_PASS, cutoff=440, resonance=0.0, peak_gain=0.0)
```

Biquad filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. Not recommended for real-time modulation; for this, use SVFilter.

