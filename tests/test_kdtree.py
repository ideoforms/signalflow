import numpy as np
import pytest
from signalflow import KDTree

def test_kdtree():
    corpus = np.random.uniform(0, 10, [1024, 2])
    tree = KDTree(corpus)
    target = np.array([1, 5])
    nearest = tree.get_nearest(target)
    assert np.all(np.isclose(target, nearest, atol=1.0))

def test_kdtree_validation():
    corpus = np.random.uniform(0, 10, [1024, 2])
    tree = KDTree(corpus)
    target = np.array([1, 2, 3])
    with pytest.raises(RuntimeError):
        nearest = tree.get_nearest(target)