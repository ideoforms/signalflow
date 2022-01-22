#------------------------------------------------------------------------
# This file is included so that pytest can find the package
# root and import the `supercollider` module from a local relative path.
#
# https://docs.pytest.org/en/latest/goodpractices.html#test-package-name
#------------------------------------------------------------------------

import os
import sys
import pytest
import sysconfig
import numpy as np
import scipy.signal

#------------------------------------------------------------------------
# Add the build path of the .so to Python's path, so that it can be
# found when running unit tests.
# https://stackoverflow.com/questions/14320220/testing-python-c-libraries-get-build-path
#------------------------------------------------------------------------
def distutils_dir_name(dir_name):
    """Returns the name of a distutils build directory"""
    f = "{dirname}.{platform}-{version[0]}.{version[1]}"
    return f.format(dirname=dir_name,
                    platform=sysconfig.get_platform(),
                    version=sys.version_info)

build_dir = os.path.join("build", distutils_dir_name("lib"))
sys.path.insert(0, build_dir)
import signalflow

DEFAULT_BUFFER_LENGTH = 1024

def process_tree(node, buffer=None, num_frames=DEFAULT_BUFFER_LENGTH):
    if buffer is not None:
        num_frames = buffer.num_frames
    for _, input in node.inputs.items():
        if input is not None:
            process_tree(input, num_frames=num_frames)

    if buffer:
        node.process(buffer)
    else:
        node.process(num_frames)

def count_zero_crossings(array):
    count = 0
    for index in range(len(array)):
        if (array[index - 1] <= 0 or index == 0) and array[index] > 0:
            count += 1
    return count

def get_peak_frequencies(samples, sample_rate):
    magnitudes = np.abs(np.fft.rfft(samples))
    peaks, _ = scipy.signal.find_peaks(magnitudes)
    peaks = peaks * sample_rate / len(samples)
    return peaks

@pytest.fixture(scope="function")
def graph():
    config = signalflow.AudioGraphConfig()
    config.sample_rate = 44100
    config.input_buffer_size = 2048
    config.output_buffer_size = 2048
    graph = signalflow.AudioGraph(config=config,
                                  output_device=signalflow.AudioOut_Dummy(2),
                                  start=False)
    return graph