from libsignal import SynthSpec, Synth
from libsignal import Sine, EnvelopeASR
from . import graph

def test_synth(graph):
    prototype = Synth()
    sine = prototype.add_node(Sine(440))
    prototype.set_output(sine)
    spec = prototype.parse()

    synth = Synth(spec)
    assert len(synth.nodes) == 2
    assert len(synth.inputs) == 0
    assert synth.output.name == "sine"
    assert synth.output.synth == synth
    assert synth.auto_free == False

    synth.auto_free = True
    assert synth.auto_free == True

def test_synth_free(graph):
    prototype = Synth()
    sine = prototype.add_node(Sine(440))
    envelope = prototype.add_node(EnvelopeASR(0.0, 0.0, 0.01))
    output = sine * envelope
    prototype.set_output(output)
    spec = prototype.parse()

    synth = Synth(spec)
    synth.auto_free = True

