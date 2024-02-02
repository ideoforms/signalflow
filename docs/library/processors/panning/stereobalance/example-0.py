from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Demonstrating the effects of StereoBalance. Two tones are first panned hard
# left and hard right with StereoPanner. 
# Setting StereoBalance's balance value to 0.0 will mean both tones are heard 
# equally. A value of -1.0 will result in only the left channel being heard. 
# A value of 1.0 will result in only the right channel being heard.
#-------------------------------------------------------------------------------

low = TriangleOscillator(220)
high = TriangleOscillator(660)

left = StereoPanner(low, -1.0)
right = StereoPanner(high, 1.0)
panned = (left + right)

balanced = StereoBalance(panned, -1.0)

balanced.play()
graph.wait()