[Reference library](../index.md) > [Processors: Panning](index.md)

# Processors: Panning

- **[AzimuthPanner](azimuthpanner/index.md)**: Pan input around an equally-spaced ring of `num_channels` speakers. `pan` is the pan position from -1..+1, where 0 = centre front. `width` is the source's width, where 1.0 spans exactly between an adjacent pair of channels.
- **[ChannelPanner](channelpanner/index.md)**: Pan the input between a linear series of channels, where `pan` 0 = channel 0, 1 = channel 1, etc. No wrapping is applied.
- **[SpatialPanner](spatialpanner/index.md)**: Implements a spatial panning algorithm, applied to a given SpatialEnvironment. Currently, only DBAP is supported.
- **[StereoBalance](stereobalance/index.md)**: Takes a stereo input and rebalances it, where `balance` of `0` is unchanged, `-1` is hard left, and `1` is hard right.
- **[StereoPanner](stereopanner/index.md)**: Pans a mono input to a stereo output. `pan` should be between -1 (hard left) to +1 (hard right), with 0 = centre.
- **[StereoWidth](stereowidth/index.md)**: Reduces the width of a stereo signal. When `width` = 1, input is unchanged. When `width` = 0, outputs a pair of identical channels both containing L+R.
