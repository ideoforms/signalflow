# The AudioGraph

`AudioGraph` is the global audio processing system that schedules and performs audio processing. It is comprised of an interconnected network of [Node](../node/index.md) and [Patch](../patch/index.md) objects, which audio flows through.

Each time a new block of audio is requested by the system audio I/O layer, the `AudioGraph` object is responsible for traversing the tree of nodes and generating new samples by calling each `Node`'s `process` method.

!!! info "Why 'Graph'?"
    You may be more familiar with "graph" being used to mean a data visualisation. In signal processing and discrete mathematics, the term "graph" is also used to denote a system of nodes related by connections. Read more: [Graph Theory Basics](https://courses.lumenlearning.com/wmopen-mathforliberalarts/chapter/introduction-graph-theory/) (Lumen Learning). 

---

[→ Next: Creating the graph](creating.md)
