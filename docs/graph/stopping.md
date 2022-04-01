# The AudioGraph

## Clearing and stopping the graph

To clear all nodes and patches from the graph but leave it running for further audio synthesis:

```
>>> graph.clear()
```

To stop the graph and pause audio I/O:

```
>>> graph.stop()
```

To permanently destroy the graph:

```
>>> graph.destroy()
```