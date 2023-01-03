# Patch

## Defining a Patch

A Patch is made up of a connected network of Nodes, together with a set of properties that determine how the Patch can be controlled.

There are two general ways to define the structure of a Patch:

- Create a new class that subclasses `Patch`. In general, this is the recommended approach for defining new Patches.
- Create a [JSON](https://www.json.org/json-en.html) file that can be loaded as a `PatchSpec`, which describes the structure of a patch 

### Creating a Patch subclass

The quickest and most intuitive way to define a `Patch` is by subclassing the `Patch` class itself. Let's look at an example.

```python
class Bleep (Patch):
    def __init__(self, frequency=880, duration=0.1):
        super().__init__()
        frequency = self.add_input("frequency", frequency)
        duration = self.add_input("duration", duration)
        sine = SineOscillator(frequency)
        env = ASREnvelope(0.001, duration, 0.001)
        output = sine * env
        self.set_output(output)
        self.set_auto_free(True)
```

In the above example:

- At the very start of the `__init__` function, `super().__init__()` must be called to initialise the Patch and its storage. **This is vital! Without it, your program will crash.** 
- Two audio-rate input parameters are defined. The `add_input()` method is used to define them as inputs of the `Patch`, which can then be subsequently modulated. Note that the `add_input()` method returns a reference to the frequency node, which then acts as a pointer to the input node.
- `self.set_output()` is used to define the Patch's output. A Patch can only have one single output.
- Finally, `self.set_auto_free()` is used to automatically stop and free the Patch after playback of the envelope is completed. [More about auto-free...](auto-free) 

You can now instantiate a `Bleep` object in just the same way as you would instantiate and play a Node:

```
b = Bleep(frequency=440, duration=0.2)
b.play()
```

If you query `graph.status` after playback has finished, you should see that the `Patch` is automatically freed and the number of nodes returns to 0. 

### Creating a PatchSpec from JSON

The structure of a `Patch` is described by a `PatchSpec`, which can in turn be imported/exported in the [JSON](https://www.json.org/json-en.html) text-based data interchange format. 

For information on loading or saving PatchSpecs as JSON, see [Exporting and importing patches](exporting).

---

[→ Next: Playing and stopping a Patch](playback)