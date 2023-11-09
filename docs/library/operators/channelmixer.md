[Reference library](../index.md) > [Operators](index.md) > [ChannelMixer](channelmixer.md)

# ChannelMixer

Downmix a multichannel input to a lower-channel output. If num_channels is greater than one, spreads the input channels across the field. If amplitude_compensation is enabled, scale down the amplitude based on the ratio of input to output channels.

Signature:
```python
ChannelMixer(num_channels=1, input=0, amplitude_compensation=true)
```
