#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: FM synthesis with MIDI keyboard trigger.
# The mido module is required for this example:
#
#   pip3 install mido python-rtmidi
#------------------------------------------------------------------------
from signalflow import *
import mido

#--------------------------------------------------------------------------------
# Parameters for each FM operator:
#   [ coarse, level, detune, envelope ]
#
# where
#   coarse is a multiple of the fundamental frequency
#   level is 0..1
#   detune is -1..+1
#   envelope is an ASR envelope with attack, sustain, release in seconds
#--------------------------------------------------------------------------------
params = [
    [ 1.0, 1.0, 0, [ 0.0, 0.3, 0.5 ] ],
    [ 2.0, 0.75, 0, [ 0.0, 0.1, 0.5 ] ],
    [ 3.5, 0.6, 0, [ 0.0, 0.1, 0.5 ] ]
]

#------------------------------------------------------------------------
# Create the global processing graph.
#------------------------------------------------------------------------
graph = AudioGraph(start=True)
graph.show_status(1)

class FM3 (Patch):
    def __init__(self, f0, params):
        super().__init__()
        op2 = FMOp(f0, *params[2])
        op1 = FMOp(f0, *params[1], fm=op2)
        op0 = FMOp(f0, *params[0], fm=op1)
        stereo = LinearPanner(2, op0 + op1)
        self.set_output(stereo * 0.1)

class FMOp (Patch):
    def __init__(self, f0, coarse, level, detune, env, fm=None):
        """
        FM operator that follows the exponential output curve of the Yamaha DX7:
        https://sound.stackexchange.com/a/42979/33063
        """
        super().__init__()
        amplitude = ScaleLinExp(level, 0, 1, 0.0005, 1)
        freq = f0 * coarse
        if fm is not None:
            freq = freq + (f0 * fm * 14)
        freq = freq * (1 + 0.0005 * detune)
        sine = SineOscillator(freq)
        env = EnvelopeASR(env[0], env[1], env[2], curve=2)
        self.set_output(sine * env * amplitude)


class MIDIVoicer:
    def __init__(self, device_name=None, voice_count=16):
        self.input = mido.open_input(device_name)
        self.voice_count = voice_count
        self.voices = [None] * voice_count
        self.voice_index = 0

    def run(self):
        for message in self.input:
            if message.type == 'note_on':
                voice = FM3(midi_note_to_frequency(message.note), params)
                voice.play()
                if self.voices[self.voice_index]:
                    self.voices[self.voice_index].stop()
                self.voices[self.voice_index] = voice
                self.voice_index = (self.voice_index + 1) % self.voice_count

voicer = MIDIVoicer()
voicer.run()
