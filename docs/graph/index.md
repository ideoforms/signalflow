# The AudioGraph

`AudioGraph` is the global audio processing graph that schedules and performs the audio processing.

It contains references to all of the [Node](../node) and [Patch](../patch) objects that are currently active, and, when a new block of audio is requested by the system audio I/O layer, traverses the tree of nodes and generates new samples.

---

[→ Next: Creating the graph](creating)
