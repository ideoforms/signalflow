from libsignal import PatchSpec, Patch
from libsignal import Sine, EnvelopeASR
from . import graph

def test_patch(graph):
    prototype = Patch()
    sine = prototype.add_node(Sine(440))
    prototype.set_output(sine)
    spec = prototype.create_spec()

    patch = Patch(spec)
    assert len(patch.nodes) == 2
    assert len(patch.inputs) == 0
    assert patch.output.name == "sine"
    assert patch.output.patch == patch
    assert patch.auto_free == False

    patch.auto_free = True
    assert patch.auto_free == True

def test_patch_free(graph):
    prototype = Patch()
    sine = prototype.add_node(Sine(440))
    envelope = prototype.add_node(EnvelopeASR(0.0, 0.0, 0.01))
    output = sine * envelope
    prototype.set_output(output)
    spec = prototype.create_spec()

    patch = Patch(spec)
    patch.auto_free = True

