from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Create a waveshaper buffer that silences any samples with amplitude < 0.5
#-------------------------------------------------------------------------------
buf = WaveShaperBuffer(lambda n: 0 if abs(n) < 0.5 else n)
sine = SineOscillator(120)
waveshaper = WaveShaper(sine, buf)
attenuated = waveshaper * 0.1
attenuated.play()