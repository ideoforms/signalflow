# Nodes

## Playing and stopping a node

### Starting playback

To start a node playing, simply call the `play()` method:

```python
graph = AudioGraph()
node = SineOscillator(440)
node.play()
```

This connects the node to the `output` node of the current global `AudioGraph`. The next time the graph processes a block of samples, the graph's `output` node then calls upon the sine oscillator to generate a block.

It is important to remember that playing a node means "connecting it to the graph". For this reason, it is not possible to play the same node more than once, as it is already connected to the graph. To play multiples of a particular Node type, simply create and play multiple instances.

### Stopping playback 

To stop a node playing:

```python
node.stop()
```

This disconnects the node from the output device that it is connected to. 

---

[→ Next: Inputs](/node/inputs)