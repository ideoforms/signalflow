from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Using a ClockDivider to create rhythms related to the main clock. Here the oscillator in the left channel is heard on every tick of the clock. The oscillator in the right channel is heard every 3 ticks of the clock.
#-------------------------------------------------------------------------------
clock = Impulse(2.0)
divided_clock = ClockDivider(clock, 3)

oscillator_a = TriangleOscillator(220)
oscillator_b = TriangleOscillator(440)

envelope_a = ASREnvelope(0.01, 0.0, 0.25, 1.0, clock)
envelope_b = ASREnvelope(0.01, 0.0, 0.5, 1.0, divided_clock)

voice_a = oscillator_a * envelope_a
voice_b = oscillator_b * envelope_b

left = StereoPanner(voice_a, -1.0)
right = StereoPanner(voice_b, 1.0)

left.play()
right.play()