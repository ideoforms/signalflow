title: RandomImpulseSequence node documentation
description: RandomImpulseSequence: Generates a random sequence of 0/1 bits with the given length, and the given probability each each bit = 1. The position of the sequence is incremented on each clock signal. explore and generate are trigger inputs which cause the sequence to mutate and re-generate respectively.

[Reference library](../../index.md) > [Stochastic](../index.md) > [RandomImpulseSequence](index.md)

# RandomImpulseSequence

Generates a random sequence of 0/1 bits with the given length, and the given probability each each bit = 1. The position of the sequence is incremented on each clock signal. explore and generate are trigger inputs which cause the sequence to mutate and re-generate respectively.

Signature:
```python
RandomImpulseSequence(probability=0.5, length=8, clock=None, explore=None, generate=None, reset=None)
```
