#!/usr/bin/env python3

from signalflow import *
import numpy as np
import datetime
import random
import os

def test_100_sines(graph):
    for n in range(100):
        sine = SineOscillator(random.uniform(100, 10000)) * 0.01
        sine.play()

def test_100_whitenoise(graph):
    for n in range(100):
        noise = WhiteNoise() * 0.01
        noise.play()

def test_beamforming(graph):
    env = SpatialEnvironment()
    num_speakers = 32
    for n in range(32):
        x, y = tuple(np.random.uniform(-1000, 1000, 2))
        env.add_speaker(n, x, y)

    noise = WhiteNoise()
    envelope = ASREnvelope(0.001, 0, 0.2, clock=Impulse(1))
    source = noise * envelope
    panner = SpatialPanner(env=env,
                           input=source,
                           x=200,
                           y=50,
                           z=500,
                           algorithm="beamformer",
                           use_delays=1)
    panner.play()

def test_fft_ifft(graph):
    noise = WhiteNoise()
    fft = FFT(noise, fft_size=4096)
    ifft = IFFT(fft)
    ifft.play()

def run_suite():
    tests = {
        "sine x 100": test_100_sines,
        "whitenoise x 100": test_100_whitenoise,
        "beamforming": test_beamforming,
        "fft -> ifft": test_fft_ifft,
    }

    render_duration = 60
    output_device = AudioOut_Dummy(num_channels=32)
    graph = AudioGraph(output_device=output_device)
    buffer = Buffer(1, graph.sample_rate * render_duration)

    for test_name, test_init_function in tests.items():
        graph.clear()
        test_init_function(graph)
        t0 = datetime.datetime.now()
        graph.render_to_buffer(buffer)
        t1 = datetime.datetime.now()
        seconds = (t1 - t0).total_seconds()
        print("%18s: Elapsed: %.2fs (%.1fx faster than realtime)" % (test_name, seconds, render_duration / seconds))

run_suite()
