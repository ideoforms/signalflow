#!/usr/bin/env python3

#--------------------------------------------------------------------------------
# Example: Buffer playback
# Reads a stereo buffer from an audio file, and plays it through system output.
#--------------------------------------------------------------------------------

import signalflow as sf
import os

graph = sf.AudioGraph(start=True)

audio_path = os.path.join(os.path.dirname(__file__), "../audio/stereo-count.wav")
buf = sf.Buffer(audio_path)
player = sf.BufferPlayer(buf, loop=True)

print("Loaded buffer: %s" % buf)

graph.play(player)
graph.wait()
