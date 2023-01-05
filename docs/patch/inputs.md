# Patch

## Patch inputs

Just like a [Node](../node/inputs.md), a Patch supports three different classes of input:

- **Audio-rate inputs**: Takes the output of another Node or Patch as an input, for continuous modulation of synthesis parameters
- **Trigger inputs**: Used to trigger discrete control events — for example, restarting buffer playback
- **Buffer inputs**: Used to pass the contents of an audio buffer to a patch — for example, as a source of audio samples, or an envelope shape

### Audio-rate inputs

A Patch supports any number of user-defined named inputs, which can be used to modulate the nodes within the patch.

Each input must be defined by calling `add_input()` when the Patch is first defined, with an optional default value.

!!! info
    Note that Patches do not yet support [variable inputs](../node/inputs.md#variable-input-nodes).

When a Patch is playing, the value of its inputs can be set using `patch.set_input()`:

```python
class Bloop (Patch):
    def __init__(self, frequency=880, duration=0.1):
        super().__init__()
        frequency = self.add_input("frequency", frequency)
        sine = SineOscillator(frequency)
        self.set_output(sine)
        self.set_auto_free(True)

bloop = Bloop()
bloop.play()
...
bloop.set_input("frequency", 100)
```

!!! info
    Note that Patches do not yet support setting inputs with Python properties (e.g. `patch.prop_name = 123`), as is possible with [node inputs](../node/inputs.md#audio-rate-inputs).

### Triggers

When defining a `Patch`, it is possible to define which Node should receive `trigger()` events sent to the Patch. This is done with `patch.set_trigger_node()`:

```python
class Hat (Patch):
    def __init__(self, duration=0.1):
        super().__init__()
        duration = self.add_input("duration", duration)
        noise = WhiteNoise()
        env = ASREnvelope(0.0001, 0.0, duration, curve=2)
        output = noise * env
        self.set_trigger_node(env)
        self.set_output(output)

h = Hat()
h.play()
...
h.trigger() # triggers a hit, resetting the ASREnvelope to its start point
```

This can be used to create a `Patch` that stays connected to the AudioGraph and can be retriggered to play a hit.

!!! info
    Note that Patches only presently support trigger events directed to a single node within the patch, and cannot route triggers to multiple different nodes.

### Buffer inputs

Buffer inputs can be declared at define time by calling `self.add_buffer_input()`. Similar to `add_input`, the return value is a placeholder `Buffer` that can be used wherever you would normally pass a `Buffer`:

```python
class WobblyPlayer (Patch):
    def __init__(self, buffer):
        super().__init__()
        buffer = self.add_buffer_input("buffer", buffer)
        rate = SineLFO(0.2, 0.5, 1.5)
        player = BufferPlayer(buffer, rate=rate, loop=True)
        self.set_output(player)

buffer = Buffer("examples/audio/stereo-count.wav")
player = WobblyPlayer(buffer)
player.play()
```

The buffer can then be replaced at runtime by calling `set_input()`:

```python
player.set_input("buffer", another_buffer)
```

---

[→ Next: Operators](operators.md)
