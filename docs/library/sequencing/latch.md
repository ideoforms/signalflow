[Reference library](../index.md) > [Sequencing](index.md) > [Latch](latch.md)

# Latch

Initially outputs 0. When a trigger is received at set, outputs 1. When a trigger is subsequently received at reset, outputs 0, until the next set.

Signature:
```python
Latch(set=0, reset=0)
```
