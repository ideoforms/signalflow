#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: FM synthesis, controlled by a MIDI input device.
# The mido module is required for this example:
#
#   pip3 install mido python-rtmidi
#------------------------------------------------------------------------
from signalflow import *
from signalflow_midi import *

#--------------------------------------------------------------------------------
# Parameters for each FM operator:
#   [ coarse, level, envelope ]
#
# where
#   coarse is a multiple of the fundamental frequency
#   level is 0..1
#   envelope is an ASR envelope with attack, sustain, release in seconds
#--------------------------------------------------------------------------------
params = [
    [ 1.00, 1.0, [ 0.0, 0.3, 0.7, 0.9 ] ],
    [ 2.0, 0.75, [ 0.0, 0.4, 0.6, 0.5 ] ],
    [ 9.1, 0.95, [ 0.0, 0.1, 0.2, 0.3 ] ]
]
lfo_rate = 4.0
lfo_am_level = 0.2
lfo_pm_level = 0.1

#------------------------------------------------------------------------
# Create the global processing graph.
#------------------------------------------------------------------------
graph = AudioGraph()
graph.poll(1)

class FM3 (Patch):
    def __init__(self, frequency, amplitude, params, gate=1):
        super().__init__()

        frequency = self.add_input("frequency", frequency)
        amplitude = self.add_input("amplitude", amplitude)
        gate = self.add_input("gate", gate)

        lfo = TriangleLFO(lfo_rate, -1, 1)
        frequency = frequency * (1.0 + 0.1 * lfo * lfo_pm_level)
        op2 = FMOp(frequency, *params[2], gate)
        op1 = FMOp(frequency, *params[1], gate, fm=op2)
        op0 = FMOp(frequency, *params[0], gate, fm=op1+op2)
        stereo = StereoPanner(op0 + op1)
        stereo = stereo * (1.0 + lfo * lfo_am_level)

        self.output = stereo * 0.1 * amplitude
        self.auto_free_node = op0.env

class FMOp (Patch):
    def __init__(self, f0, coarse, level, env, gate, fm=None):
        """
        FM operator that follows the exponential output curve of the Yamaha DX7:
        https://sound.stackexchange.com/a/42979/33063
        """
        super().__init__()
        amplitude = ScaleLinExp(level, 0, 1, 0.0005, 1)
        frequency = f0 * coarse
        if fm is not None:
            frequency = frequency + (f0 * fm * 14)
        sine = SineOscillator(frequency)
        self.env = ADSREnvelope(*env, gate=gate)
        self.output = sine * (self.env ** 2) * amplitude

#--------------------------------------------------------------------------------
# MIDIManager is part of the signalflow_midi helper package.
# This snippet specifies that the "FM3" patch should be used to play individual
# MIDI voices when a key is pressed.
#
# The patch must support parameters named `frequency` and `gate`.
#
# To use a specific MIDI input device, replace None below with the device name. 
#--------------------------------------------------------------------------------
manager = MIDIManager(device_name=None)
manager.set_voice_patch(FM3, params=params)
graph.wait()
