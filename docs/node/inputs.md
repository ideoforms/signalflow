# Nodes

## Node inputs

A node has three different classes of input:

- **Audio-rate inputs**: Takes the output of another node as an input, for continuous modulation of synthesis parameters
- **Trigger inputs**: Used to trigger discrete control events — for example, restarting buffer playback
- **Buffer inputs**: Used to pass the contents of an audio buffer to a node — for example, as a source of audio samples, or an envelope shape

---

### Audio-rate inputs

Virtually every node has one or more audio-rate inputs. Put simply, an audio-rate input is **the output of another node**. Let's look at a short example:

```python
lfo = SineLFO()
signal = SquareOscillator(frequency=200, width=lfo)
```

In this case, we are passing the output of a `SineLFO` as the pulse width of a `SquareOscillator`. This is an audio-rate input.

Although it's not obvious, the `frequency` parameter is also an audio-rate input. Any constant value (such as the `200` here) is behind the scenes implemented as a `Constant` node, which continuously outputs the value at an audio rate.

All audio-rate inputs can be modified just like a normal Python property. For example:

```python
signal.frequency = TriangleOscillator(0.5, 100, 1000)
```

---

#### Variable input nodes

Some nodes have a variable number of inputs, which can change over the Node's lifetime. For example, `Sum()` takes an arbitrary number of input Nodes, and generates an output which is the sum of all of its inputs.

For variable-input nodes such as this, audio-rate inputs are added with `add_input()`, and can be removed with `remove_input()`.

```python
a = Constant(1)
b = Constant(2)
c = Constant(3)
sum = Sum()
sum.add_input(a)
sum.add_input(b)
sum.add_input(c)
# sum will now generate an output of 6.0
```

It is possible to check whether a Node object takes variable inputs by querying `node.has_variable_inputs`.

---

### Triggers

When working with sequencing and timing, it is often useful be able to trigger discrete events within a node. This is where trigger inputs come in handy.

There are two different ways to handle trigger inputs:

- by calling the `trigger()` method on a `Node`
- by passing a Node to an input that corresponds to an audio-rate trigger

#### Calling trigger()

To generate trigger events at arbitrary times, call `node.trigger()`. For example:

```python
freq_env = Line(10000, 100, 0.5)
sine = SineOscillator(freq_env)
sine.play()
while True:
    freq_env.trigger()
    graph.wait(1)
```

This is useful because it can be done outside the audio thread. For example, `trigger()` could be called each time a MIDI note event is received. 

The `trigger()` method takes an optional `name` parameter, which is used by `Node` classes containing more than one type of trigger. This example uses the `set_position` trigger of `BufferPlayer` to seek to a new location in the sample every second. 

```python
buffer = Buffer("../audio/stereo-count.wav")
player = BufferPlayer(buffer, loop=True)
player.play()
while True:
    player.trigger("set_position", random_uniform(0, buffer.duration))
    graph.wait(1)
```

!!! note
    Because the `trigger` method happens outside the audio thread, it will take effect at the start of the next audio block. This means that, if you are running at 44.1kHz with an audio buffer size of 1024 samples, this could introduce a latency of up to `1024/44100 = 0.023s`.   For time-critical events like drum triggers, this can be minimised by reducing the [hardware output buffer size](../graph/config.md).
    
    This constraint also means that only one event can be triggered per audio block. To trigger events at a faster rate than the hardware buffer size allows, see **Audio-rate triggers** below.  

#### Audio-rate triggers

It is often desirable to trigger events using the audio-rate output of another Node object as a source of trigger events, to give sample-level precision in timing. Most nodes that support `trigger` inputs can also be triggered by a corresponding audio-rate input. 

Triggers happen at zero-crossings — that is, when the output of the node passes above zero (i.e., from `<= 0` to `>0`). For example, to create a clock with an oscillating tempo to re-trigger buffer playback: 

```python
clock = Impulse(SineLFO(0.2, 1, 10))
buffer = Buffer("examples/audio/stereo-count.wav")
player = BufferPlayer(buffer, loop=True, clock=clock)
player.play()
```

This can be used to your advantage with the boolean operator nodes. 

```python
on_the_right = MouseX() > 0.5
envelope = ASREnvelope(0, 0, 0.5, clock=on_the_right)
square = SquareOscillator(100)
output = envelope * square * 0.1
output.play()
```

*TODO: Should the name of the trigger() event always be identical to the trigger input name? So `clock` for envelopes, buffer player, etc...?*

---

### Buffer inputs

The third type of input supported by nodes is the [buffer](../buffer/index.md). Nodes often take buffer inputs as sources of audio samples. They are also useful as sources of envelope shape data (for example, to shape the grains of a Granulator), or general control data (for example, recording motion patterns from a `MouseX` input).

```python
buffer = Buffer("../audio/stereo-count.wav")
player = BufferPlayer(buffer, loop=True)
```

---

[→ Next: Operators](operators.md)
