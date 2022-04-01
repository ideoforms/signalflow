# The AudioGraph

## Creating the graph

Creating the graph is simple: `graph = AudioGraph()`

By default, a new `AudioGraph` immediately connects to the system's default audio hardware device (via the integrated `libsoundio` library), using the system's default sample rate and buffer size.

!!! info
    Note that the AudioGraph is a singleton object: only one AudioGraph can be created, which is shared globally.

To prevent the graph from starting instantly (for example, if you want to use the graph in offline mode), pass `start=False` to the constructor.

To configure graph playback or recording parameters, see [AudioGraph: Configuration](/graph/config).

---

[→ Next: Graph configuration](/graph/config)