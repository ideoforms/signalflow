# Nodes

!!! warning
    This documentation is a work-in-progress and may have sections that are missing or incomplete.

A `Node` object is an audio processing unit that performs one single function. For example, a Node's role may be to synthesize a waveform, read from a buffer, or take two input Nodes and sum their values.

## Playing a node

To start a node playing, simply call the `play()` method:

```
>>> graph = AudioGraph(start=True)
>>> node = SineOscillator()
>>> node.play()
```

An oscillator node like this does not actually have a "playing" state. The `play()` method simply connects it to the `output` node of the current global `AudioGraph`. The next time the graph processes a block of samples, the graph's `output` node then calls upon the sine oscillator to generate a block.

!!! info
    `node.play()` is actually syntactical sugar for `graph.play(node)`, which is also equivalent to `graph.output.add_input(node)`. These alternative formulations are useful in rare cases in which you have more than one graph. 

To stop a node playing:

```
>>> node.stop()
```

This disconnects the node from the output device that it is connected to. 

## Node inputs

A node has three different types of of input:

- **Audio-rate inputs**: Uses the output of another node as an input, for continuous modulation of synthesis parameters
- **Triggers and trigger**: Used to trigger discrete control events -- for example, restarting buffer playback
- **Buffer inputs**: Used to pass the contents of an audio buffer to a node -- for example, as a source of audio samples, or an envelope shape

### Audio-rate inputs

Virtually every node has one or more audio-rate inputs. Put simply, an audio-rate input is **the output of another node**. Let's look at a short example:

```python
lfo = SineLFO()
signal = SquareOscillator(frequency=200, width=lfo)
```

In this case, we are passing the output of a `SineLFO` as the pulse width of a `SquareOscillator`. This is an audio-rate input.

Although it's not obvious, the `frequency` parameter is also an audio-rate input. Any constant value (such as the `200` here) is behind the scenes re-cast as a `Constant`, which endlessly outputs the value at an audio rate.

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

This is handy because it can be done outside the audio thread (for example, in response to MIDI events). 

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
    Because the `trigger` method happens outside the audio thread, it will take effect at the start of the next audio block. This means that, if you are running at 44.1kHz with an audio buffer size of 1024 samples, this could introduce a latency of up to `1024/44100 = 0.023s`.   For time-critical events like drum triggers, this can be minimised by reducing your [hardware buffer size](/graph/).
    
    This constraint also means that only one event can be triggered per audio block. To trigger events at a faster rate than the hardware buffer size allows, see **Audio-rate triggers** below.  

#### Audio-rate triggers

It is often desirable to trigger events using the audio-rate output of another Node object as a source of trigger events, to give sample-level precision in timing. Most nodes that support `trigger` inputs can also be triggered by a corresponding audio-rate input. 

Triggers happen at zero-crossings -- that is, when the output of the node passes above zero (i.e., from `<= 0` to `>0`). For example, to create a clock with an oscillating tempo to re-trigger buffer playback: 

```python
clock = Impulse(SineLFO(0.2, 1, 10))
buffer = Buffer("../audio/stereo-count.wav")
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

*TODO: Implement and document `Difference`*

*TODO: Should the name of the trigger() event always be identical to the trigger input name? So `clock` for envelopes, buffer player, etc...?*

### Buffer inputs

The third type of input supported by nodes is the [buffer](/buffer/). Nodes often take buffer inputs as sources of audio samples. They are also useful as sources of envelope shape data (for example, to shape the grains of a Granulator).

```python
TODO
``` 

## Multichannel nodes

When passing a value to audio-rate input of a Node, the signal is by default monophonic (single-channel). For example, `SquareOscillator(440)` generates a 1-channel output.

It is possible to generate multi-channel output by passing an array of values in the place of a constant. For example, `SquareOscillator([440, 880])` generates stereo output with a different frequency in the L and R channels. 

There is no limit to the number of channels that can be generated by a node. For example, `SquareOscillator(list(100 + 50 * n for n in range(100)))` will create a node with 100-channel output, each with its own frequency. 

```python
>>> sq = SquareOscillator(list(100 + 50 * n for n in range(100)))
>>> print(sq.num_output_channels)
100
```

### Automatic upmixing

There are generally multiple inputs connected to a node, which may themselves have differing number of channels. For example, `SquareOscillator(frequency=[100, 200, 300, 400, 500], width=0.7)` has a 5-channel input an a 1-channel input. In cases like this, the output of the nodes with fewer channels is *upmixed* to match the higher-channel inputs.

Upmixing here means simply duplicating the output until it reaches the desired number of channels. In the above case, the `width` input will be upmixed to generate 5 channels, all containing `0.7`.

If `width` were a stereo input with L and R channels, the output would be tiled, alternating between the channels. Each frame of stereo input would then be upmixed to contain `[L, R, L, R, L]`, where `L` and `R` are the samples corresponding to the L and R channels.

The key rule is that, for nodes that support upmixing, the **output signal has as many channels as the input signal with the highest channel count**. 

This process percolates through the signal chain. For example:

```python
SquareOscillator(frequency=SineLFO([1, 3, 5], min=440, max=880),
                 width=SawLFO([0.5, 0.6], min=0.25, max=0.75))
```

- The `min` and `max` inputs of the `frequency` LFO would be upmixed to 3 channels each
- The `min` and `max` inputs of the `width` LFO would be upmixed to 2 channels each
- Then, the output of the `width` node would be upmixed from 2 to 3 channels

### Nodes with fixed input/output channels

### The Channel* node classes

There are a number of Node subclasses dedicated to channel handling.

* **ChannelArray**: Concatenates the channels of multiple nodes, so that calling `ChannelMix` with nodes of `N` and `M` channels will produce an output of `N + M` channels.
* **ChannelMixer**: Reduces or expands the number of channels by evenly spreading the audio across the output channels.
* **ChannelSelect**: Selects sub-channels of the input, either individually or by group. 

### Querying channel subsets with the index operator


## Triggers
## Buffers

## Monitoring node values

To monitor the output of a node, call `node.poll(num_seconds)`, where `num_seconds` is the interval between messages. This will print the last sample generated by the node to `stdout`. In the case of multichannel nodes, only the first channel's value is printed.

```
>>> a = Counter(Impulse(1))
>>> a.poll(1)
>>> a.play()
counter: 0.00000
counter: 1.00000
counter: 2.00000
```

To stop polling a node, call `node.poll(0)`.

## Chance and stochastic nodes
## Nodes without output
## Node class reference
## Developing new Node classes

See [CONTRIBUTING.md](https://github.com/ideoforms/signalflow/blob/master/CONTRIBUTING.md)
