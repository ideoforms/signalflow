# Nodes

## Playing and stopping a node

### Starting playback

To start a node playing, simply call the `play()` method:

```python
graph = AudioGraph()
node = SineOscillator(440)
node.play()
```

This connects the node to the `output` endpoint of the current global `AudioGraph`. The next time the graph processes a block of samples, the graph's `output` node then calls upon the sine oscillator to generate a block.

It is important to remember that playing a node means "connecting it to the graph". For this reason, it is not possible to play the same node more than once, as it is already connected to the graph. To play multiples of a particular Node type, simply create and play multiple instances.

### Connecting a Node to another Node's input

It is often the case that you want to connect a Node to the input of another Node for playback, rather than simply wiring it to the output of a graph -- for example, to pass an oscillator through a processor. In this case, you do not need to call `play()` (which means "connect this node to the graph"). Instead, it is sufficient to simply connect the Node to the input of another Node that is already playing.

For example:

```python
# create and begin playback of a variable input summer, passed through a filter
sum = Sum()
flt = SVFilter(sum, "low_pass", 200)
flt.play()
```

Now, let's create an oscillator. Observe that connecting the oscillator to the filter's input begins playback immediately.

```python
square = SquareOscillator(100)
sum.add_input(square)
``` 

### Stopping playback 

To stop a node playing:

```python
node.stop()
```

This disconnects the node from the output device that it is connected to. 

---

[→ Next: Inputs](inputs.md)
