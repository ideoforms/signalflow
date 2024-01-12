[Reference library](../index.md) > [Oscillators](index.md)

# Oscillators

- **[Impulse](impulse/index.md)**: Produces a value of 1 at the given `frequency`, with output of 0 at all other times. If frequency is 0, produces a single impulse.
- **[SawLFO](sawlfo/index.md)**: Produces a sawtooth LFO, with output ranging from `min` to `max`.
- **[SawOscillator](sawoscillator/index.md)**: Produces a (non-band-limited) sawtooth wave, with the given `frequency` and `phase` offset. When a `reset` or trigger is received, resets the phase to zero.
- **[SineLFO](sinelfo/index.md)**: Produces a sinusoidal LFO at the given `frequency` and `phase` offset, with output ranging from `min` to `max`.
- **[SineOscillator](sineoscillator/index.md)**: Produces a sine wave at the given `frequency`.
- **[SquareLFO](squarelfo/index.md)**: Produces a pulse wave LFO with the given `frequency` and pulsewidth of `width`, ranging from `min` to `max`, where `width` of `0.5` is a square wave.
- **[SquareOscillator](squareoscillator/index.md)**: Produces a pulse wave with the given `frequency` and pulse `width`, where `width` of `0.5` is a square wave and other `width` values produce a rectangular wave.
- **[TriangleLFO](trianglelfo/index.md)**: Produces a triangle LFO with the given `frequency` and `phase` offset, ranging from `min` to `max`.
- **[TriangleOscillator](triangleoscillator/index.md)**: Produces a triangle wave with the given `frequency`.
- **[Wavetable](wavetable/index.md)**: Plays the wavetable stored in buffer at the given `frequency` and `phase` offset. `sync` can be used to provide a hard sync input, which resets the wavetable's phase at each zero-crossing.
- **[Wavetable2D](wavetable2d/index.md)**: Wavetable2D
