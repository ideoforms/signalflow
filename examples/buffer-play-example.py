#!/usr/bin/env python3

#--------------------------------------------------------------------------------
# Example: Buffer playback
# Reads a stereo buffer from an audio file, and plays it through system output.
#--------------------------------------------------------------------------------

from signalflow import *
import os

graph = AudioGraph()

#--------------------------------------------------------------------------------
# Read samples into a Buffer by passing the filename
#--------------------------------------------------------------------------------
audio_path = os.path.join(os.path.dirname(__file__), "audio", "stereo-count.wav")
buf = Buffer(audio_path)
print("Loaded buffer with %d channel(s), duration: %.1fs" % (buf.num_channels, buf.duration))

#--------------------------------------------------------------------------------
# Begin looping playback
#--------------------------------------------------------------------------------
player = BufferPlayer(buf, loop=True)

graph.play(player)
graph.wait()
