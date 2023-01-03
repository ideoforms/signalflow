# The AudioGraph

## Recording the audio output of the graph

Convenience methods are provided to make it easy to record the global audio output when rendering audio in real-time:

```python
graph.start_recording("filename.wav")
...
graph.stop_recording()
```

To record output in formats other than the default stereo, `start_recording` takes a `num_channels` argument that can be used to specify an alternative channel count.

!!! note
    At present, only .wav is supported as an output format for global audio recordings. 

## Offline (non-real-time) rendering

It is also possible to perform non-real-time rendering of a synthesis graph,  by synthesizing audio output to a `Buffer` which can then be saved to disk:   

```python
# Create an AudioGraph with a dummy output device
graph = AudioGraph(output_device=AudioOut_Dummy(2))

# Create a buffer that will be used to store the audio output
buffer = Buffer(2, graph.sample_rate * 4)

# Create a synthesis graph to render
freq = SawLFO(1, 200, 400)
sine = SineOscillator([freq, freq+10])
graph.play(sine)

# Render to the buffer. Non-real-time, so happens instantaneously.
graph.render_to_buffer(buffer)

# Write the buffer contents to a file
buffer.save("output.wav")

# Finally, tear down the buffer
graph.destroy()
```

---

[→ Next: Clearing and stopping the graph](stopping)