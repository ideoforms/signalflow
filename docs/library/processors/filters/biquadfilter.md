[Reference library](../index.md) > [Processors: Filters](index.md) > [BiquadFilter](biquadfilter.md)

# BiquadFilter

Biquad filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. Not recommended for real-time modulation; for this, use SVFilter.

Signature:
```python
BiquadFilter(input=0.0, filter_type=SIGNALFLOW_FILTER_TYPE_LOW_PASS, cutoff=440, resonance=0.0, peak_gain=0.0)
```