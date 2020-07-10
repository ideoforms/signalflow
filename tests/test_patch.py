from signalflow import PatchSpec, Patch, Buffer, BufferPlayer
from signalflow import Multiply, Sine, EnvelopeASR
from . import graph
import numpy as np

def test_patch(graph):
    prototype = Patch()
    buf_in = prototype.add_buffer_input("buffer")
    input = prototype.add_input("input")
    player = prototype.add_node(BufferPlayer(buf_in, loop=True))
    output = prototype.add_node(Multiply(input, player))
    prototype.set_output(output)
    spec = prototype.create_spec()

    patch = Patch(spec)
    buf_in = Buffer([ 2, 2, 2, 2 ])
    patch.set_input("input", 123)
    patch.set_input("buffer", buf_in)
    assert len(patch.nodes) == 5
    assert len(patch.inputs) == 1
    assert patch.output.name == "multiply"
    assert patch.output.patch == patch
    assert patch.auto_free == False

    graph.add_output(patch)
    b = Buffer(1, 1000)
    graph.render_to_buffer(b)
    assert np.all(b.data[0] == 246)

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

