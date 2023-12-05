title: ImpulseSequence node documentation
description: ImpulseSequence: Each time a `clock` or trigger is received, outputs the next value in `sequence`. At all other times, outputs zero.

[Reference library](../../index.md) > [Sequencing](../index.md) > [ImpulseSequence](index.md)

# ImpulseSequence

```python
ImpulseSequence(sequence=std::vector<int> ( ), clock=None)
```

Each time a `clock` or trigger is received, outputs the next value in `sequence`. At all other times, outputs zero.

