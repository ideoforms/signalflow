from libsignal import SynthTemplate, SynthSpec, Synth
from libsignal import Sine
from . import graph

def test_synth(graph):
    synth_template = SynthTemplate("template")
    sine = synth_template.add_node(Sine(440))
    synth_template.set_output(sine)
    synth_spec = synth_template.parse()
    assert synth_spec.name == "template"

    synth = Synth(synth_spec)
    assert len(synth.nodes) == 2
    assert len(synth.inputs) == 0
    assert synth.output.name == "sine"
    assert synth.output.synth == synth
    assert synth.auto_free == False

    synth.auto_free = True
    assert synth.auto_free == True