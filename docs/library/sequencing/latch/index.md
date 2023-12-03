title: Latch node documentation
description: Latch: Initially outputs 0. When a trigger is received at `set`, outputs 1. When a trigger is subsequently received at `reset`, outputs 0, until the next `set`.

[Reference library](../../index.md) > [Sequencing](../index.md) > [Latch](index.md)

# Latch

```python
Latch(set=0, reset=0)
```

Initially outputs 0. When a trigger is received at `set`, outputs 1. When a trigger is subsequently received at `reset`, outputs 0, until the next `set`.

