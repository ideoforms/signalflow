# Patch

## Playing a Patch

Once a `Patch` has been defined or imported, it can be instantiated in two different ways depending on how it was defined:

 - [From a Patch subclass](#from-a-patch-subclass)
 - [From a PatchSpec](#from-a-patchspec)

### From a Patch subclass

The simplest way to instantiate a Patch is by defining it as a Patch subclass, and then instantiating it in the same way as a Node. 

```python
class Hat (Patch):
    def __init__(self, duration=0.1):
        super().__init__()
        duration = self.add_input("duration", duration)
        noise = WhiteNoise()
        env = ASREnvelope(0.0001, 0.0, duration, curve=2)
        output = noise * env
        self.set_output(output)
        self.set_auto_free(True)

hat = Hat()
hat.play()
```

Once a Patch has finished, its state changes to `SIGNALFLOW_PATCH_STATE_STOPPED`.

Just as with nodes, it is important to remember that playing a patch means "connecting it to the graph". For this reason, it is not possible to play the same patch more than once, as it is already connected to the graph.

To play multiples of a particular `Patch` type, simply create and play multiple instances.

### From a PatchSpec

Once a `PatchSpec` has been created or imported, it can be played by instantiating a `Patch` with the `PatchSpec` as an argument:

```python
patch = Patch(patch_spec)
patch.play()
```

### Connecting a Patch to another Patch's input

A `Patch` can be connected to the input of another `Patch` (or Node), in exactly the same way described in [Connecting a Node to another Node's input](../node/playback#connecting-a-node-to-another-nodes-input).

Once you have got to grips with this paradigm, it becomes simple to build up sophisticated processing graphs by abstracting complex functionality within individual `Patch` objects, and connecting them to one another. 

## Stopping a Patch

As in [Node playback](../node/playback), stopping a Patch disconnects it from the AudioGraph. Patches with [auto-free](auto-free) are automatically stopped when their lifetimes ends. Patches with an unlimited lifespan must be stopped manually, with:

```python
patch.stop()
```

This disconnects the Patch from its output.

---

[→ Next: Patch inputs](inputs)