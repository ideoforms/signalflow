# Arithmetic operators

Buffers, [like nodes](../node/operators.md), can be manipulated using Python's standard arithmetic operators.

For example, to attenuate a buffer, it can be multiplied by a constant value. A new `Buffer` object is returned, with the same dimensions as the original, scaled by the coefficient.

```python
input_buffer = Buffer("input.wav")
scaled_buffer = input_buffer * 0.5
# `scaled_buffer` now contains an attenuated version of `input_buffer`
```

Below is a full list of operators supported by SignalFlow `Buffer` objects.

| Operator | Node class |
|----------|------------|
| `+`      | Add        |
| `-`      | Subtract   |
| `*`      | Multiply   |
| `/`      | Divide     |

---

[→ Next: Buffer properties](properties.md)