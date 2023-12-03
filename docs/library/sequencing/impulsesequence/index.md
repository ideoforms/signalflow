title: ImpulseSequence node documentation
description: ImpulseSequence: Each time a `clock` or trigger is received, outputs the next value in `sequence`. At all other times, outputs zero.

[Reference library](../../index.md) > [Sequencing](../index.md) > [ImpulseSequence](index.md)

# ImpulseSequence

Each time a `clock` or trigger is received, outputs the next value in `sequence`. At all other times, outputs zero.

Signature:
```python
ImpulseSequence(sequence=std::vector<int> ( ), clock=None)
```
