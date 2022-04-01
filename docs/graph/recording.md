# The AudioGraph

## Recording the audio output of the graph

Convenience methods are provided to make it easy to record the global audio output:

```python
graph.start_recording("filename.wav")
...
graph.stop_recording()
```

To record output in formats other than the default stereo, `start_recording` takes a `num_channels` argument that can be used to specify an alternative channel count.

!!! note
    At present, only .wav is supported as an output format for global audio recordings. 

---

[→ Next: Clearing and stopping the graph](/graph/stopping)