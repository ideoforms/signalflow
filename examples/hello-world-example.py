#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Hello World example.
#
# Play a sine tone.
#------------------------------------------------------------------------
from signalflow import *

def main():
    #------------------------------------------------------------------------
    # Create the global processing graph.
    #------------------------------------------------------------------------
    graph = AudioGraph()

    #------------------------------------------------------------------------
    # Create a sine oscillator, attenuate by 12dB, and pan to stereo.
    #------------------------------------------------------------------------
    sine = SineOscillator(440)
    sine = sine * db_to_amplitude(-12)
    stereo = StereoPanner(sine)

    #------------------------------------------------------------------------
    # Play the
    #------------------------------------------------------------------------
    graph.play(stereo)
    graph.wait(2)

if __name__ == "__main__":
    main()