from signalflow import PatchSpec, Patch, Buffer, BufferPlayer
from signalflow import Multiply, SineOscillator, ASREnvelope, SquareOscillator, Sum, Add, AzimuthPanner, StereoPanner
from . import graph
import numpy as np
import json

def test_patch(graph):
    prototype = Patch()
    buf_in = prototype.add_buffer_input("buffer")
    input = prototype.add_input("input")
    player = prototype.add_node(BufferPlayer(buf_in, loop=True))
    output = prototype.add_node(Multiply(input, player))
    prototype.set_output(output)
    spec = prototype.to_spec()

    patch = Patch(spec)
    buf_in = Buffer([ 2, 2, 2, 2 ])
    patch.set_input("input", 123)
    patch.set_input("buffer", buf_in)
    assert len(patch.nodes) == 5
    assert len(patch.inputs) == 1
    assert patch.output.name == "multiply"
    assert patch.output.patch == patch
    assert patch.auto_free == False

    graph.play(patch)
    b = Buffer(1, 1000)
    graph.render_to_buffer(b)
    assert np.all(b.data[0] == 246)

    patch.auto_free = True
    assert patch.auto_free == True

    graph.stop(patch)

def test_patch_duplicate(graph):
    patch = Patch()
    a = patch.add_input("a", 1)
    b = patch.add_input("b", 2)
    c = patch.add_input("c", 3)
    sum = patch.add_node(Sum([a, b, c]))
    patch.set_output(sum)
    duplicate = Patch(patch.to_spec())
    duplicate.set_input("a", 1)
    duplicate.set_input("b", 2)
    duplicate.set_input("c", 3)
    graph.play(duplicate)
    buf = Buffer(1, 100)
    assert np.all(buf.data[0] == 0)
    graph.render_to_buffer(buf)
    assert np.all(buf.data[0] == 6)

def test_patch_free(graph):
    prototype = Patch()
    sine = prototype.add_node(SineOscillator(440))
    envelope = prototype.add_node(ASREnvelope(0.0, 0.0, 0.01))
    output = sine * envelope
    prototype.set_output(output)
    spec = prototype.to_spec()

    patch = Patch(spec)
    patch.auto_free = True
    # TODO

def test_patch_property_serialisation(graph):
    """
    Test that an int Property is properly serialised and instantiated.
    """
    prototype = Patch()
    panner = AzimuthPanner(3, 0)
    prototype.set_output(panner)
    spec = prototype.to_spec()

    patch = Patch(spec)
    assert patch.output.num_output_channels == 3
    assert patch.output.get_property("num_channels") == 3

def test_patch_property_serialisation_json(graph):
    prototype = Patch()
    panner = AzimuthPanner(3, 0)
    prototype.set_output(panner)
    spec = prototype.to_spec()
    spec_json = spec.to_json()
    spec_json_obj = json.loads(spec_json)

    assert spec_json_obj["nodes"][0]["properties"]["num_channels"] == 3

    spec_new = PatchSpec()
    spec_new.from_json(spec_json)
    patch = Patch(spec_new)
    assert patch.output.num_output_channels == 3

def test_patch_to_json(graph):
    class TestPatch (Patch):
        def __init__(self):
            super().__init__()
            sin = SineOscillator(440) * ASREnvelope()
            stereo = StereoPanner(sin)
            self.set_output(stereo)

    patch = TestPatch()
    spec = patch.to_spec()
    spec_json = spec.to_json()
    ref_json = '{"buffer_inputs": [], "inputs": [], "name": "", "nodes": [{"id": 0, "inputs": {"input": {"id": 1}, "pan": 0}, "is_output": true, "node": "stereo-panner"}, {"id": 1, "inputs": {"input0": {"id": 2}, "input1": {"id": 4}}, "node": "multiply"}, {"id": 2, "inputs": {"frequency": 440}, "node": "sine"}, {"id": 4, "inputs": {"attack": 0.1, "curve": 1, "release": 0.1, "sustain": 0.5}, "node": "asr-envelope"}]}'

    #--------------------------------------------------------------------------------
    # Can't simply compare the to_json output to the ref_json, as ordering of objects
    # is not necessarily preserved (because json11 internally represents an object as
    # a std::pair which can't guarantee order). At some point, it would be nice to
    # switch to a json library that can preserve order, so that node IDs are
    # sequenced correctly. For now, make the test permissive.
    #--------------------------------------------------------------------------------
    spec_json_deserialised = json.loads(spec_json)
    ref_json_deserialised = json.loads(ref_json)
    assert spec_json_deserialised == ref_json_deserialised
