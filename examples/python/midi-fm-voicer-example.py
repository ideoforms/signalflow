#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: FM synthesis, controlled by a MIDI input device.
# The mido module is required for this example:
#
#   pip3 install mido python-rtmidi
#------------------------------------------------------------------------
from signalflow import *
import mido

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
    [ 0.25, 1.0, [ 0.0, 0.3, 0.7, 0.9 ] ],
    [ 2.0, 0.75, [ 0.0, 0.4, 0.7, 0.7 ] ],
    [ 8.17, 0.9, [ 0.0, 0.1, 0.7, 1.8 ] ]
]

#------------------------------------------------------------------------
# Create the global processing graph.
#------------------------------------------------------------------------
graph = AudioGraph(start=True)
graph.show_status(1)

class FM3 (Patch):
    def __init__(self, frequency, params):
        super().__init__()
        gate = self.add_input("gate", 1)
        op2 = FMOp(frequency, *params[2], gate)
        op1 = FMOp(frequency, *params[1], gate, fm=op2)
        op0 = FMOp(frequency, *params[0], gate, fm=op1+op2)
        stereo = LinearPanner(2, op0 + op1)
        self.set_output(stereo * 0.1)

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
        env = EnvelopeADSR(*env, gate=gate)
        self.set_output(sine * env * amplitude)


class MIDIVoicer:
    def __init__(self, device_name=None):
        self.input = mido.open_input(device_name)
        self.notes = [None] * 128

    def run(self):
        for message in self.input:
            if message.type == 'note_on':
                voice = FM3(midi_note_to_frequency(message.note), params)
                voice.play()
                voice.auto_free = True
                self.notes[message.note] = voice
            elif message.type == 'note_off':
                if self.notes[message.note]:
                    self.notes[message.note].set_input("gate", 0)
                    self.notes[message.note] = None

voicer = MIDIVoicer()
voicer.run()
