from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using OneTapDelay to create a delay effect with no feedback.
# The original sound is heard in the left channel, and the delayed sound in the
# right channel.
#-------------------------------------------------------------------------------
clock = Impulse(1)
oscillator = TriangleOscillator(440)
envelope = ASREnvelope(0.001, 0, 0.3, 1.0, clock)
voice = oscillator * envelope
delayed = OneTapDelay(voice, 0.25) * 0.5
output = ChannelArray([voice, delayed]) * 0.5
output.play()
graph.wait()