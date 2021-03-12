#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: FM synthesis with MIDI keyboard trigger.
# The mido module is required for this example:
#
#   pip3 install mido python-rtmidi
#------------------------------------------------------------------------
from signalflow import *
import mido

#------------------------------------------------------------------------
# Create the global processing graph.
#------------------------------------------------------------------------
graph = AudioGraph(start=True)
graph.show_status(1)

class FM3 (Patch):
    def __init__(self, f0, params):
        super().__init__()
        p0 = params[0]
        p1 = params[1]
        p2 = params[2]
        op2 = FMOp(f0, p2[0], detune=p2[2], env=p2[3])
        op1 = FMOp(f0, p1[0], detune=p1[2], env=p1[3])
        op0 = FMOp(f0, p0[0], op1 + op2, p0[1], detune=p0[2], env=p0[3])
        stereo = LinearPanner(2, op0)
        self.set_output(stereo)

class FMOp (Patch):
    def __init__(self, f0, coarse, fm=None, fm_amp=None, detune=0, env=[0.25, 1, 1]):
        super().__init__()
        freq = f0 * coarse
        if fm is not None:
            fm_amp = ScaleLinExp(fm_amp, 0, 1, 0.01, 100)
            freq = freq + (fm * f0 * fm_amp)
        freq = freq * (1 + 0.0005 * detune)
        sine = SineOscillator(freq)
        env = EnvelopeASR(env[0], env[1], env[2], curve=2)
        self.set_output(sine * env * 0.1)

params = [
    [ 0.5, 0.73, 0, [ 0.0, 0.3, 0.5 ] ],
    [ 3, 0.79, 0, [ 0.0, 0.1, 0.3 ] ],
    [ 1.0, 1.0, 0, [ 0.0, 0.1, 0.2 ] ]
]

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
