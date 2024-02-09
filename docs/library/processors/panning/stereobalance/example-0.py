from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Demonstrating the effects of StereoBalance. First a low tone is assigned to 
# the left channel and a high tone is assigned to the right channel.
# Setting StereoBalance's balance value to 0.0 will mean both tones are heard 
# equally. A value of -1.0 will result in only the left channel being heard. 
# A value of 1.0 will result in only the right channel being heard.
# In this example, an LFO is modulating the balance value between -1.0 and 1.0.
#-------------------------------------------------------------------------------
low = TriangleOscillator(220)
high = TriangleOscillator(660)
panned = ChannelArray([low, high])
balanced = StereoBalance(panned, TriangleLFO(0.2, -1, 1)) * 0.5
balanced.play()
graph.wait()