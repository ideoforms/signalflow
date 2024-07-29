[Reference library](../index.md) > [Operators](index.md)

# Operators

- **[Add](add/index.md)**: Add each sample of a to each sample of b. Can also be written as a + b
- **[AmplitudeToDecibels](amplitudetodecibels/index.md)**: Map a linear amplitude value to decibels.
- **[DecibelsToAmplitude](decibelstoamplitude/index.md)**: DecibelsToAmplitude
- **[ChannelArray](channelarray/index.md)**: Takes an array of inputs and spreads them across multiple channels of output.
- **[ChannelCrossfade](channelcrossfade/index.md)**: Given a multichannel input, crossfades between channels based on the given position within the virtual array, producing a single-channel output.
- **[ChannelMixer](channelmixer/index.md)**: Downmix a multichannel input to a lower-channel output. If num_channels is greater than one, spreads the input channels across the field. If amplitude_compensation is enabled, scale down the amplitude based on the ratio of input to output channels.
- **[ChannelSelect](channelselect/index.md)**: Select a subset of channels from a multichannel input, starting at offset, up to a maximum of maximum, with the given step.
- **[Equal](equal/index.md)**: Compares the output of a to the output of b. Outputs 1 when equal, 0 otherwise. Can also be written as a == b
- **[NotEqual](notequal/index.md)**: Compares the output of a to the output of b. Outputs 0 when equal, 1 otherwise. Can also be written as a != b
- **[GreaterThan](greaterthan/index.md)**: Compares the output of a to the output of b. Outputs 1 when a > b, 0 otherwise. Can also be written as a > b
- **[GreaterThanOrEqual](greaterthanorequal/index.md)**: Compares the output of a to the output of b. Outputs 1 when a >= b, 0 otherwise. Can also be written as a >= b
- **[LessThan](lessthan/index.md)**: Compares the output of a to the output of b. Outputs 1 when a < b, 0 otherwise. Can also be written as a < b
- **[LessThanOrEqual](lessthanorequal/index.md)**: Compares the output of a to the output of b. Outputs 1 when a <= b, 0 otherwise. Can also be written as a <= b
- **[Modulo](modulo/index.md)**: Outputs the value of a modulo b, per sample. Supports fractional values. Can also be written as a % b
- **[Abs](abs/index.md)**: Outputs the absolute value of a, per sample. Can also be written as abs(a)
- **[If](if/index.md)**: Outputs value_if_true for each non-zero value of a, value_if_false for all other values.
- **[Divide](divide/index.md)**: Divide each sample of a by each sample of b. Can also be written as a / b
- **[FrequencyToMidiNote](frequencytomidinote/index.md)**: Map a frequency to a MIDI note (where 440Hz = A4 = 69), with floating-point output.
- **[MidiNoteToFrequency](midinotetofrequency/index.md)**: Map a MIDI note to a frequency (where 440Hz = A4 = 69), supporting floating-point input.
- **[Multiply](multiply/index.md)**: Multiply each sample of a by each sample of b. Can also be written as a * b
- **[Pow](pow/index.md)**: Outputs a to the power of b, per sample. Can also be written as a ** b
- **[RoundToScale](roundtoscale/index.md)**: Given a frequency input, generates a frequency output that is rounded to the nearest MIDI note. (TODO: Not very well named)
- **[Round](round/index.md)**: Round the input to the nearest integer value.
- **[ScaleLinExp](scalelinexp/index.md)**: Scales the input from a linear range (between a and b) to an exponential range (between c and d).
- **[ScaleLinLin](scalelinlin/index.md)**: Scales the input from a linear range (between a and b) to a linear range (between c and d).
- **[Subtract](subtract/index.md)**: Subtract each sample of b from each sample of a. Can also be written as a - b
- **[Sum](sum/index.md)**: Sums the output of all of the input nodes, by sample.
- **[TimeShift](timeshift/index.md)**: TimeShift
- **[TriggerMult](triggermult/index.md)**: Distribute any triggers to all output nodes.
- **[Sin](sin/index.md)**: Outputs sin(a), per sample.
- **[Cos](cos/index.md)**: Outputs cos(a), per sample.
- **[Tan](tan/index.md)**: Outputs tan(a), per sample.
- **[Tanh](tanh/index.md)**: Outputs tanh(a), per sample. Can be used as a soft clipper.
