from signalflow import AudioGraph, AudioGraphConfig
from . import graph
import pytest

def test_config(graph):
    assert graph.config.sample_rate >= 0
