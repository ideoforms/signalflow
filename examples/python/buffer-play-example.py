#!/usr/bin/env python3

#--------------------------------------------------------------------------------
# Example: Buffer playback
# Reads a stereo buffer from an audio file, and plays it through system output.
#--------------------------------------------------------------------------------

import signalflow as sf

graph = sf.AudioGraph()

buf = sf.Buffer("../audio/stereo-count.wav")
player = sf.BufferPlayer(buf, loop=True)

print("Loaded buffer: %s" % buf)

graph.play(player)
graph.start()
graph.wait()
