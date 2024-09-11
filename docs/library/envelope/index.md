[Reference library](../index.md) > [Envelope](index.md)

# Envelope

- **[Accumulator](accumulator/index.md)**: Accumulator with decay.
- **[ADSREnvelope](adsrenvelope/index.md)**: Attack-decay-sustain-release envelope. Sustain portion is held until gate is zero.
- **[ASREnvelope](asrenvelope/index.md)**: Attack-sustain-release envelope.
- **[DetectSilence](detectsilence/index.md)**: Detects blocks of silence below the threshold value. Used as an auto-free node to terminate a Patch after processing is complete.
- **[Envelope](envelope/index.md)**: Generic envelope constructor, given an array of levels, times and curves.
- **[Line](line/index.md)**: Line segment with the given start/end values, and duration (in seconds). If loop is true, repeats indefinitely. Retriggers on a clock signal.
- **[RectangularEnvelope](rectangularenvelope/index.md)**: Rectangular envelope with the given sustain duration.
