#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Use a MIDI keyboard to trigger patch voices.
# The mido module is required for this example:
#
#   pip3 install mido python-rtmidi
#
# For help on MIDI devices, run this example with the -h flag.
#------------------------------------------------------------------------
import sys
try:
    import mido
except ModuleNotFoundError:
    print("This example requires mido: pip3 install mido python-rtmidi")
    sys.exit(1)

import numpy as np
import argparse
from signalflow import *

class NotePatch (Patch):
    def __init__(self):
        super().__init__()
        note = self.add_input("note", 60)
        amplitude = self.add_input("amplitude", 0.5)
        gate = self.add_input("gate", 1.0)
        freq = MidiNoteToFrequency(note)
        env = ADSREnvelope(0.001, 0.5, 0.9, 0.2, gate=gate)
        wavetable = self.add_buffer_input("wavetable")
        signal = Wavetable(wavetable, freq)
        signal = Resample(signal, bit_rate=3)
        filter_env = ADSREnvelope(0.001, 0.1, 0.5, 0.5, gate=gate) ** 4
        filter_env = ScaleLinExp(filter_env, 0, 1, 800, 8000)
        filter_lfo = SineLFO(RandomUniform(3, 8, Impulse(0)), 0.7, 1.3)
        resonance = RandomUniform(0.5, 0.99, Impulse(0))
        signal = SVFilter(signal, SIGNALFLOW_FILTER_TYPE_LOW_PASS, filter_env * filter_lfo, resonance)
        lfo = SineLFO(8, 0.6, 1.0)
        pan = RandomUniform(-0.5, 0.5, Impulse(0))
        output = StereoPanner(signal * env * amplitude * lfo * 0.2, pan)
        output = Resample(output, bit_rate=8)
        self.set_output(output)

def make_wavetable():
    """
    Make a warm-sounding wavetable buffer, using the first 12 harmonics
    of a sawtooth wave.
    """
    wavetable_size = 1024
    saw = np.zeros(wavetable_size)
    for n in range(1, 12):
        saw += np.sin(np.arange(wavetable_size) * n * np.pi * 2.0 / wavetable_size) / n
    return Buffer(1, wavetable_size, np.array([ saw ]))

def main(args):
    graph = AudioGraph()
    graph.poll(0.5)

    #--------------------------------------------------------------------------------
    # If the -l flag is passed, list MIDI input devices
    #--------------------------------------------------------------------------------
    if args.list_devices:
        print("Input devices:")
        for device in mido.get_input_names():
            print(" - %s" % device)
        sys.exit(0)

    default_input = mido.open_input(args.device_name)
    print(" - Opened input: %s" % default_input)

    #--------------------------------------------------------------------------------
    # Create a re-usable patch specification,
    # and an array of voice holders to store the currently-played voices.
    #--------------------------------------------------------------------------------
    buf = make_wavetable()
    patch = NotePatch()
    spec = patch.to_spec()
    voices = [ None ] * 128
    print(spec.to_json())

    try:
        for message in default_input:
            #--------------------------------------------------------------------------------
            # Each time a MIDI note event is received, either trigger or release
            # the appropriate voice.
            #--------------------------------------------------------------------------------
            if message.type == 'note_on':
                voice = Patch(spec)
                voice.set_input("note", message.note)
                voice.set_input("amplitude", message.velocity / 127)
                voice.set_input("wavetable", buf)
                voice.auto_free = True
                voice.play()
                voices[message.note] = voice
            elif message.type == 'note_off':
                if voices[message.note] is not None:
                    voices[message.note].set_input("gate", 0)
                    voices[message.note] = None
                else:
                    print("No patch found for note %d" % message.note)
    except KeyboardInterrupt:
        print("Stopping playback")
        default_input.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Use MIDI input to play synth voices using SignalFlow")
    parser.add_argument("-i", "--device-name", help="MIDI input device name to use")
    parser.add_argument("-l", "--list-devices", action="store_true", help="List MIDI input devices and exit")
    args = parser.parse_args()
    main(args)
