# Nodes

## Playing and stopping a node

To start a node playing, simply call the `play()` method:

```python
graph = AudioGraph()
node = SineOscillator(440)
node.play()
```

This connects the node to the `output` node of the current global `AudioGraph`. The next time the graph processes a block of samples, the graph's `output` node then calls upon the sine oscillator to generate a block.

To stop a node playing:

```python
node.stop()
```

This disconnects the node from the output device that it is connected to. 

---

[→ Next: Inputs](/node/inputs)