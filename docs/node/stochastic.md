# Nodes

## Chance and stochastic nodes

SignalFlow has a number of stochastic nodes, which make use of a pseudo-random number generator (RNG) to produce unpredictable output values.

Each object of these `StochasticNode` subclasses stores its own RNG. By default, the RNG is seeded with a random value, so that each run will generate a different set of outputs. However, to create a repeatable pseudo-random output, the `seed` of the node's RNG can be set to a known value:

```python
>>> r = RandomUniform(0, 1)
>>> r.process(1024)
>>> r.output_buffer[0][:4]
array([0.48836085, 0.64326525, 0.79819506, 0.8489549 ], dtype=float32)
>>> r.set_seed(123)
>>> r.process(1024)
>>> r.output_buffer[0][:4]
array([0.7129553 , 0.42847094, 0.6908848 , 0.7191503 ], dtype=float32)
>>> r.set_seed(123)
>>> r.process(1024)
>>> r.output_buffer[0][:4]
array([0.7129553 , 0.42847094, 0.6908848 , 0.7191503 ], dtype=float32)
```

Note the identical sequences generated after repeatedly setting the seed to a known value. 

!!! warning
    Calling `node.process()` is generally not good practice, as it does not recursively process all of the node's inputs (unlike when a node is embedded within an AudioGraph, which correctly handles recursion and cyclical loops). Please use at your peril!

---

[→ Next: Node reference library](library)