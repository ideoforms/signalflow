[Reference library](../index.md) > [Stochastic](index.md)

# Stochastic

- **[Logistic](logistic/index.md)**: Logistic noise.
- **[PinkNoise](pinknoise/index.md)**: Pink noise, with specified low/high cutoffs.
- **[RandomBrownian](randombrownian/index.md)**: Outputs Brownian noise between min/max, with a mean change of delta between samples. If a clock is passed, only generates a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomChoice](randomchoice/index.md)**: Pick a random value from the given array. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomCoin](randomcoin/index.md)**: Flip a coin with the given probability. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomExponentialDist](randomexponentialdist/index.md)**: Generate an random value following the exponential distribution. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomExponential](randomexponential/index.md)**: Generate an random exponential value between min/max. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomGaussian](randomgaussian/index.md)**: Generate an random Gaussian value, with given mean and sigma. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[RandomImpulseSequence](randomimpulsesequence/index.md)**: Generates a random sequence of 0/1 bits with the given length, and the given probability each each bit = 1. The position of the sequence is incremented on each clock signal. explore and generate are trigger inputs which cause the sequence to mutate and re-generate respectively.
- **[RandomImpulse](randomimpulse/index.md)**: Generate random impulses at the given frequency, with either uniform or poisson distribution.
- **[RandomUniform](randomuniform/index.md)**: Generates a uniformly random value between min/max. If a clock is passed, only picks a new value on a clock tick. To generate and hold an unchanging random value, pass clock=0.
- **[WhiteNoise](whitenoise/index.md)**: Generates whitenoise between min/max. If frequency is zero, generates at audio rate. For frequencies lower than audio rate, interpolate applies linear interpolation between values, and random_interval specifies whether new random values should be equally-spaced or randomly-spaced.
