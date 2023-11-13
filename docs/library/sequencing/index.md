[Reference library](../index.md) > [Sequencing](index.md)

# Sequencing

- **[ClockDivider](clockdivider.md)**: When given a clock input (e.g., an Impulse), divides the clock by the given factor. factor must be an integer greater than or equal to 1.
- **[Counter](counter.md)**: Count upwards from min, driven by clock.
- **[Euclidean](euclidean.md)**: Euclidean rhythm as described by Toussaint, with sequence_length (n) and num_events (k), driven by clock.
- **[FlipFlop](flipflop.md)**: Flips from 0/1 on each clock.
- **[ImpulseSequence](impulsesequence.md)**: Each time a clock or trigger is received, outputs the next value in the sequence. At all other times, outputs zero.
- **[Index](index_.md)**: Outputs the value in list corresponding to index.
- **[Latch](latch.md)**: Initially outputs 0. When a trigger is received at set, outputs 1. When a trigger is subsequently received at reset, outputs 0, until the next set.
- **[Sequence](sequence.md)**: Outputs the elements in sequence, incrementing position on each clock.
