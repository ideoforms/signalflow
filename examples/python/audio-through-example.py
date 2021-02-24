#!/usr/bin/env python3

#--------------------------------------------------------------------------------
# Example: Audio playthrough, from input to output.
# (Be careful to listen through headphones or feedback might occur.)
#--------------------------------------------------------------------------------

from signalflow import *

graph = AudioGraph(start=True)

#--------------------------------------------------------------------------------
# Read single channel input.
# To specify the audio device to use, add to ~/.signalflow/config:
# 
# [audio]
# input_device_name = "My Device"
#--------------------------------------------------------------------------------
audio_in = AudioIn()

#--------------------------------------------------------------------------------
# Add some ringmod and delay.
#--------------------------------------------------------------------------------
audio_out = audio_in * Sine(200)
audio_out = audio_out + CombDelay(audio_out, 0.2, 0.8) * 0.3
output = ChannelArray([ audio_out, audio_out ])

graph.play(output)
graph.wait()
