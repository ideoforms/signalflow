[Reference library](../index.md) > [Processors: Delays](index.md)

# Processors: Delays

- **[AllpassDelay](allpassdelay/index.md)**: All-pass delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.
- **[CombDelay](combdelay/index.md)**: Comb delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.
- **[OneTapDelay](onetapdelay/index.md)**: Single-tap delay line. `delay_time` must be less than or equal to `max_delay_time`.
- **[Stutter](stutter/index.md)**: Stutters the input whenever a trigger is received on `clock`. Generates `stutter_count` repeats, with duration of `stutter_time`.
