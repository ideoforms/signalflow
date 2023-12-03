import matplotlib.pyplot as plt
import numpy as np
from signalflow import *

config = AudioGraphConfig()
config.output_buffer_size = 4096
graph = AudioGraph(output_device=AudioOut_Dummy(buffer_size=2048),
                   config=config)

osc = SawOscillator(500)
osc.play()
buf = graph.render_to_new_buffer(4410)

#--------------------------------------------------------------------------------
# Plot graphs
#--------------------------------------------------------------------------------
fig, axs = plt.subplots(nrows=2, figsize=(12, 5), dpi=200)
axs[0].plot(buf.data[0])
axs[1].magnitude_spectrum(buf.data[0], Fs=graph.sample_rate)

plt.show()
