#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Sequencing example.
#
# Complex sequencing is best achieved using a counterpart library
# that is specifically designed for control sequences
# (e.g. https://github.com/ideoforms/isobar). For simple sequences,
# SignalFlow includes several nodes for producing patterns and structure
# within the DSP graph.
#------------------------------------------------------------------------
from signalflow import *
import numpy as np

#------------------------------------------------------------------------
# Create the global processing graph.
#------------------------------------------------------------------------
graph = AudioGraph()
graph.poll(1)

class SquareArpPatch (Patch):
    #------------------------------------------------------------------------
    # Simple patch that iterates through a sequence of MIDI notes
    # following the given clock
    #------------------------------------------------------------------------
    def __init__(self, clock, seq, fundamental, pan=0.0):
        super().__init__()
        sequence = Sequence(np.array(seq), clock)
        frequency = MidiNoteToFrequency(sequence + fundamental)
        square = SquareOscillator(frequency, width=SineLFO([0.1, 0.11], 0.05, 0.5))
        filter_env = ScaleLinExp(ASREnvelope(0.001, 0.01, 0.1, clock=clock) ** 3, 0, 1, 100, 5000)
        filtered = SVFilter(square, "low_pass", filter_env, resonance=0.2) * 0.2
        balanced = StereoBalance(filtered, pan)
        self.set_output(balanced)

class HiHatPatch (Patch):
    #------------------------------------------------------------------------
    # Minimum viable hi-hat
    #------------------------------------------------------------------------
    def __init__(self, clock, amp=1.0):
        super().__init__()
        noise = WhiteNoise()
        noise = SVFilter(noise, "high_pass", 8000)
        env = ASREnvelope(0.001, 0.01, 0.1, clock=clock)
        pan = SineLFO(0.5, -0.3, 0.3)
        output = StereoPanner(noise * env * amp, pan)
        self.set_output(output)

#------------------------------------------------------------------------
# Create a 16th-beat clock
#------------------------------------------------------------------------
bpm = 110
clock = Impulse(4 * bpm/60)

#------------------------------------------------------------------------
# Shared sequence that is used to transpose MIDI notes
#------------------------------------------------------------------------
fundamental = Sequence([ 0, -4, -5 ], Impulse(1/4))

#------------------------------------------------------------------------
# Create three parallel arpeggios
#------------------------------------------------------------------------
graph.play(SquareArpPatch(clock, np.array([ 0, 0, 0, 0, 7, 12, 0, 7 ]) + 36, fundamental))
graph.play(SquareArpPatch(clock, np.array([ 3, 2, 0, -2, 0]) + 60, fundamental, pan=-0.75))
graph.play(SquareArpPatch(clock, np.array([ 7, 3, 5, 2, 3, 0 ]) + 72, fundamental, pan=0.75))

#------------------------------------------------------------------------
# Add a simple hi-hat sequence
#------------------------------------------------------------------------
amp_sequence = Sequence([ 0.1, 0.25, 0.5, 0.1 ], clock=clock)
graph.play(HiHatPatch(clock, amp=amp_sequence))

#------------------------------------------------------------------------
# Run indefinitely.
#------------------------------------------------------------------------
graph.wait()
