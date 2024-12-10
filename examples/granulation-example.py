#!/usr/bin/env python3

#------------------------------------------------------------------------
# SignalFlow: Granulation example.
#------------------------------------------------------------------------
from signalflow import *
import os

def main():
    #------------------------------------------------------------------------
    # Create the global processing graph.
    #------------------------------------------------------------------------
    graph = AudioGraph()
    graph.poll(1)

    #------------------------------------------------------------------------
    # Load an audio buffer.
    #------------------------------------------------------------------------
    audio_path = os.path.join(os.path.dirname(__file__), "audio", "stereo-count.wav")
    buf = Buffer(audio_path)
    grain_frequency = WhiteNoise(frequency=2, min=1, max=50)
    clock = RandomImpulse(grain_frequency)
    granulator = Granulator(buffer=buf,
                            clock=clock,
                            pan=RandomUniform(-1, 1, clock=clock),
                            pos=RandomUniform(0, buf.duration, clock=clock),
                            rate=RandomExponential(0.5, 2.0, clock=clock),
                            duration=0.5)
    envelope = EnvelopeBuffer("triangle")
    granulator.set_buffer("envelope", envelope)

    #------------------------------------------------------------------------
    # Play the output.
    #------------------------------------------------------------------------
    graph.play(granulator)
    graph.wait()

if __name__ == "__main__":
    main()
