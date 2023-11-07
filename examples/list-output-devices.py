#!/usr/bin/env python3

#--------------------------------------------------------------------------------
# SignalFlow: List available audio output devices
#--------------------------------------------------------------------------------

from signalflow import *

graph = AudioGraph(start=False)
output_devices = graph.output_device_names
print("Available output devices:")
for output_device in output_devices:
    print(f" - {output_device}")
