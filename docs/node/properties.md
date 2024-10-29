# Nodes

## Node properties

A `Node` has a number of read-only properties which can be used to query its status at a given moment in time.

| Property               | Type          | Description                                                                                                                                                                                                                                                                                                                                                                                            |
|------------------------|---------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| name                   | str           | Short alphanumeric string that identifies the type of node (for example, `asr-envelope`)                                                                                                                                                                                                                                                                                                               |
| num_output_channels    | int           | The number of output channels that the node generates.                                                                                                                                                                                                                                                                                                                                                 |
| num_input_channels     | int           | The number of input channels that the node takes. Note that most nodes have `matches_input_channels` set, meaning that their `num_input_channels` will be automatically increased according to their inputs. To learn more, see [Nodes: Multichannel](multichannel.md).                                                                                                                                | 
| matches_input_channels | bool          | Whether the node automatically increases its `num_input_channels` based on its inputs. To learn more, see [Nodes: Multichannel](multichannel.md).                                                                                                                                                                                                                                                      |
| has_variable_inputs    | bool          | Whether the node supports an [arbitrary number of audio-rate inputs](inputs.md#variable-input-nodes)                                                                                                                                                                                                                                                                                                   |
| output_buffer          | numpy.ndarray | Contains the Node's most recent audio output, in `float32` samples. The buffer is indexed by `channel` x `frame`, so to obtain the 32nd sample in the first channel, query: `node.output_buffer[0][31]`.                                                                                                                                                                                               |
| inputs                 | dict          | A dict containing all of the `Node`'s audio-rate inputs. Note that buffer inputs are not currently included within this dict.                                                                                                                                                                                                                                                                          |
| state                  | int           | The Node's current playback state, which can be one of `SIGNALFLOW_NODE_STATE_ACTIVE` and `SIGNALFLOW_NODE_STATE_STOPPED`. The `STOPPED` state only applies to those nodes which have a finite duration (e.g. `ASREnvelope`, or `BufferPlayer` with looping disabled) and have reached the end of playback. Nodes continue to have a state of `ACTIVE` whether or not they are connected to the graph. | 
| patch                  | Patch         | Indicates the [Patch](../patch/index.md) that the node is part of, or None if the Node does not belong to a Patch.                                                                                                                                                                                                                                                                                     | 

### Monitoring a node's output

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

### Node-specific properties

Some `Node` classes have additional properties, containing information on implementation-specific states. These can be accessed via the `get_property` method.

For example, the `BufferPlayer` node exposes a `position` property, which returns the playhead's current position, in seconds.

```
>>> buffer = Buffer("audio.wav")
>>> player = BufferPlayer(buffer)
>>> player.play()
...
>>> player.get_property("position")
5.984000205993652
```

---

[→ Next: Stochastic nodes](stochastic.md)