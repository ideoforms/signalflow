#!/usr/bin/env python3

#--------------------------------------------------------------------------------
# SignalFlow: Karplus-Strong string synthesis.
# https://en.wikipedia.org/wiki/Karplus%E2%80%93Strong_string_synthesis
#--------------------------------------------------------------------------------

from signalflow import *
import numpy as np
import random

feedback = 0.99
note_interval = 0.125

#--------------------------------------------------------------------------------
# Karplus-Strong synthesis requires a short delay line whose delay time
# is 1/F of the synthesized frequency. Because SignalFlow reads and writes
# buffers (including the feedback notes) on a block-by-block basis, this delay
# time can only be as short as the graph's global block size, so set a small
# block size so we can render a reasonable range of frequencies.
#--------------------------------------------------------------------------------
config = AudioGraphConfig()
config.output_buffer_size = 128
graph = AudioGraph(config=config, start=True)
graph.poll(2)

#--------------------------------------------------------------------------------
# Short burst of noise to excite the delay line
#--------------------------------------------------------------------------------
excitation_buf = Buffer(np.random.uniform(-1, 1, 256))
player = BufferPlayer(excitation_buf)

#--------------------------------------------------------------------------------
# Set up a buffer that will be used as the feedback line's delay ringbuffer.
#--------------------------------------------------------------------------------
feedback_buf = Buffer(1, 2048)
feedback_reader = FeedbackBufferReader(feedback_buf)

#--------------------------------------------------------------------------------
# Pick a random frequency each time a note is played. If `clock` is non-zero,
# RandomExponential will generate a random signal at audio rate. Zero it and
# it will only generate when triggered.
#--------------------------------------------------------------------------------
random_frequency = RandomExponential(1, 6, clock=0)
rounded_frequency = Round(random_frequency) * 50

#--------------------------------------------------------------------------------
# Write to the feedback buffer.
#--------------------------------------------------------------------------------
output = SVFilter(player * 0.5 + feedback_reader, "low_pass", 4000)
feedback_writer = FeedbackBufferWriter(feedback_buf, output * feedback, 1/rounded_frequency)

#--------------------------------------------------------------------------------
# Play the rendered output as a stereo signal.
# Note that the feedback_writer also needs to be "played". This is because
# the audio synthesis works by pulling each of the Graph's outputs in turn,
# and recursing through each of their inputs. As the FeedbackBufferWriter
# is not an input to any other node, it must be processed explicitly.
#--------------------------------------------------------------------------------
graph.play([output, output])
graph.play(feedback_writer)

#--------------------------------------------------------------------------------
# Periodically, re-generate the excitation buffer and generate a new note.
#--------------------------------------------------------------------------------
while True:
    graph.wait(note_interval)
    excitation_buf.fill(lambda n: random.uniform(-1, 1))
    player.trigger()
    random_frequency.trigger()
