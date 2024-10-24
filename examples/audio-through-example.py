#!/usr/bin/env python3

# --------------------------------------------------------------------------------
# Example: Audio playthrough, from input to output.
# (Listen through headphones to avoid feedback)
# --------------------------------------------------------------------------------

from signalflow import *


def main():
    graph = AudioGraph()

    #--------------------------------------------------------------------------------
    # Read mono audio input.
    # To specify the audio device to use, add to ~/.signalflow/config:
    #
    # [audio]
    # input_device_name = "My Device"
    #--------------------------------------------------------------------------------
    audio_in = AudioIn(1)

    #--------------------------------------------------------------------------------
    # Add some delay, and play 
    #--------------------------------------------------------------------------------
    output = audio_in
    stereo = StereoPanner(output)

    graph.play(stereo)
    graph.wait()


if __name__ == "__main__":
    main()
