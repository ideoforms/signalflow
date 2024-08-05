import os
import sys
import pytest
import sysconfig
import numpy as np
import scipy.signal

#------------------------------------------------------------------------
# Set a standard buffer size to use throughout most unit tests.
# This is needed because some functionality (e.g. pitch detection)
# relies on having a minimum buffer size to work reliably.
#------------------------------------------------------------------------
SIGNALFLOW_UNIT_TEST_BUFFER_SIZE = 2048
SIGNALFLOW_UNIT_TEST_SAMPLE_RATE = 44100

#------------------------------------------------------------------------
# Add the build path of the .so to Python's path, so that it can be
# found when running unit tests.
# https://stackoverflow.com/questions/14320220/testing-python-c-libraries-get-build-path
#------------------------------------------------------------------------
def distutils_dir_name(dir_name):
    """
    Returns the name of a distutils build directory
    Requires setuptools >= 62.1.0
    https://stackoverflow.com/a/14369968/1819404
    """
    f = "{dirname}.{platform}-{cache_tag}"
    return f.format(dirname=dir_name,
                    platform=sysconfig.get_platform(),
                    cache_tag=sys.implementation.cache_tag)

#------------------------------------------------------------------------
# Ensure that build dir exists and prioritise this library in sys.path
#------------------------------------------------------------------------
build_dir = os.path.join("build", distutils_dir_name("lib"))
if os.path.exists(build_dir) and build_dir not in sys.path:
    sys.path.insert(0, build_dir)

#------------------------------------------------------------------------
# Ensure that placeholder package from auxiliary isn't imported
#------------------------------------------------------------------------
sys.path = list(filter(lambda path: "auxiliary/libs" not in path, sys.path))

import signalflow

def process_tree(node, buffer=None, num_frames=signalflow.SIGNALFLOW_NODE_BUFFER_SIZE):
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
    config.sample_rate = SIGNALFLOW_UNIT_TEST_SAMPLE_RATE
    config.input_buffer_size = SIGNALFLOW_UNIT_TEST_BUFFER_SIZE
    config.output_buffer_size = SIGNALFLOW_UNIT_TEST_BUFFER_SIZE
    graph = signalflow.AudioGraph(config=config,
                                  output_device=signalflow.AudioOut_Dummy(2, SIGNALFLOW_UNIT_TEST_BUFFER_SIZE),
                                  start=False)
    yield graph
    #--------------------------------------------------------------------------------
    # Ensure that graph is forcibly destroyed and cleared from memory even if the
    # test fails (which can lead dangling references). Not doing this means that
    # all subsequent tests will fail with "AudioGraph has already been created".
    #--------------------------------------------------------------------------------
    graph.destroy()
