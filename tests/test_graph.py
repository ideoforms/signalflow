import sys
sys.path.append("build/lib.macosx-10.14-x86_64-3.7")
import pytest
import libsignal

def test_graph():
    graph = libsignal.AudioGraph()
    assert graph
