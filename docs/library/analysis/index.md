[Reference library](../index.md) > [Analysis](index.md)

# Analysis

- **[CrossCorrelate](crosscorrelate/index.md)**: Outputs the cross-correlation of the input signal with the given buffer. If hop_size is zero, calculates the cross-correlation every sample.
- **[NearestNeighbour](nearestneighbour/index.md)**: Nearest Neighbour.
- **[OnsetDetector](onsetdetector/index.md)**: Simple time-domain onset detector. Outputs an impulse when an onset is detected in the input. Maintains short-time and long-time averages. An onset is registered when the short-time average is threshold x the long-time average. min_interval is the minimum interval between onsets, in seconds.
- **[VampAnalysis](vampanalysis/index.md)**: Feature extraction using the Vamp plugin toolkit.
