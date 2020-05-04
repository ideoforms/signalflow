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
import libsignal

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

DEFAULT_BUFFER_LENGTH = 1024

def process_tree(node, num_frames=DEFAULT_BUFFER_LENGTH):
    for _, input in node.inputs.items():
        process_tree(input, num_frames)
    node.process(num_frames)

def count_zero_crossings(array):
    count = 0
    for index in range(len(array)):
        if array[index - 1] <= 0 and array[index] > 0:
            count += 1
    return count

@pytest.fixture(scope="module")
def graph():
    graph = libsignal.AudioGraph()
    yield graph
    del graph
    graph = None
