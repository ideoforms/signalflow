from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using AllpassDelay to add a delay effect to a simple melodic sequence.
# The original oscillator can be heard in the left channel.
# The delay effect can be heard in the right channel.
#-------------------------------------------------------------------------------
clock = Impulse(1.0)
sequence = Sequence([ 60, 62, 64, 65, 67, 69, 71, 72 ], clock)
frequency = MidiNoteToFrequency(sequence)

oscillator = TriangleOscillator(frequency)
envelope = ASREnvelope(0, 0.2, 0.3, 1.0, clock)
voice = oscillator * envelope
delayed = AllpassDelay(input=voice, 
                       delay_time=0.4, 
                       feedback=0.8)

output = ChannelArray([ voice, delayed ]) * 0.75
output.play()
graph.wait()