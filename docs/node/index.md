# Nodes

A `Node` object is an audio processing unit that performs one single function. For example, a Node's role may be to synthesize a waveform, read from a buffer, or take two input Nodes and sum their values.

- Nodes are [played and stopped](playback.md) by connecting them to the AudioGraph 
- A node has one or more [audio-rate inputs](inputs.md#audio-rate-inputs), which can be modulated by other nodes
- Some nodes can be triggered with [trigger inputs](inputs.md#triggers) — for example, to restart playback, or set the position of an envelope
- Some nodes can be used to play back the contents of [buffer inputs](inputs.md#buffer-inputs), or can use buffer data as a source of modulation
- The output of multiple nodes can be combined and modulated with use of the standard Python [operators](operators.md) (`+`, `-`, `*`, `%`, etc)
- The output of a node can be mono (single-channel) or [multichannel](multichannel.md)
- A Node's status and output can be examined by querying its [properties](properties.md)
- Some Nodes generate unpredictable [stochastic output](stochastic.md), which can be controlled via its internal random number generator 
- Details of how to create a new Node type are detailed in [Developing a new Node class](developing.md)

For an overview of every type of Node available in SignalFlow, see the [Node Reference Library](../library/index.md)

---

[→ Next: Node playback](playback.md)
