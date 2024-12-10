#!/usr/bin/env python3

# --------------------------------------------------------------------------------
# Example: Audio playthrough, from input to output.
# (Listen through headphones to avoid feedback)
# --------------------------------------------------------------------------------

from signalflow import *


def main():
    print("Audio playthrough: Plays the microphone input through the default output")
    print("Listen through headphones to avoid feedback")

    graph = AudioGraph()

    #--------------------------------------------------------------------------------
    # Read mono audio input.
    # To specify the audio device to use, add to ~/.signalflow/config:
    #
    # [audio]
    # input_device_name = "My Device"
    #--------------------------------------------------------------------------------
    audio_in = AudioIn()
    audio_in = audio_in[0]

    #--------------------------------------------------------------------------------
    # Add some delay, and play 
    #--------------------------------------------------------------------------------
    output = audio_in
    output = output + CombDelay(output, 0.1, feedback=0.8)
    stereo = StereoPanner(output)

    graph.play(stereo)
    graph.wait()


if __name__ == "__main__":
    main()
