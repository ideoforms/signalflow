# Nodes

## Node operators

### Arithmetic

The output of multiple nodes can be combined using Python's mathematical operators. For example, to sum two sine waves together to create harmonics, use the `+` operator:

```python
output = SineOscillator(440) + SineOscillator(880)
output.play()
```

To modulate the amplitude of one node with another, use the `*` operator:

```python
sine = SineOscillator(440)
envelope = ASREnvelope(0.1, 1, 0.1)
output = sine * envelope
```

You can use constant values in place of `Node` objects:

```python
sine = SineOscillator(440)
attenuated = sine * 0.5
```

Operators can be chained together in the normal way:

```python
# Create an envelope that rises from 0.5 to 1.0 and back to 0.5
env = (ASREnvelope(0.1, 1, 0.1) * 0.5) + 0.5
```

Behind the scenes, these operators are actually creating composites of `Node` subclasses. The last example could alternatively be written as:

```python
Add(Multiply(ASREnvelope(0.1, 1, 0.1), 0.5), 0.5)
```

### Comparison

Comparison operators can also be used to compare two Node output values, generating a binary (1/0) output. For example:

```python
# Generates an output of 1 when the sinusoid is above 0, and 0 otherwise 
SineOscillator(440) > 0
```

This can then be used as an input to other nodes. The below will generate a half-wave-rectified sine signal (that is, a sine wave with all negative values set to zero). 

```python
sine = SineOscillator(440)
rectified = sine * (sine > 0)
```

### Index of operators

Below is a full list of operators supported by SignalFlow.

#### Arithmetic operators

| Operator | Node class |
|----------|------------|
| `+`      | Add        |
| `-`      | Subtract   |
| `*`      | Multiply   |
| `/`      | Divide     |
| `**`     | Power      |
| `%`      | Modulo     |

#### Comparison operators

| Operator | Node class         |
|----------|--------------------|
| `==`     | Equal              |
| `!=`     | NotEqual           |
| `<`      | LessThan           |
| `<=`     | LessThanOrEqual    |
| `>`      | GreaterThan        |
| `>=`     | GreaterThanOrEqual |


---

[→ Next: Multichannel](multichannel)