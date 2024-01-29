# Saving and exporting a buffer

## Saving to a sound file

To export a buffer's audio contents to a sound file, use the `save()` method:

```python
import numpy as np
buf = Buffer(np.sin(np.linspace(0, 1, graph.sample_rate) * 440 * np.pi * 2))
buf.save("buffer.wav")
```

The output format will be automatically detected from the filename extension. Supported formats are presently `wav`, `aif` and `flac`.

---

[→ Next: Passing a buffer as an input to a node or patch](input.md)