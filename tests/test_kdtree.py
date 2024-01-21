import numpy as np
import pytest
from signalflow import KDTree

def test_kdtree():
    corpus = np.random.uniform(0, 10, [1024, 2])
    tree = KDTree(corpus)
    target = np.array([1, 5])
    nearest = tree.get_nearest(target)

    # check that the nearest value found is sufficiently near to the target
    assert np.all(np.isclose(target, nearest.coordinate, atol=1.0))

    # check that nearest.index points to the item in the corpus identical to nearest.coordinate
    # (subject to rounding error)
    assert np.all(np.isclose(corpus[nearest.index], nearest.coordinate, atol=1e-6))

    # check that nearest.distance calculates the correct distance
    # (subject to rounding error)
    assert nearest.distance == pytest.approx(np.linalg.norm(corpus[nearest.index] - target), abs=1e-6)

def test_kdtree_validation():
    corpus = np.random.uniform(0, 10, [1024, 2])
    tree = KDTree(corpus)
    target = np.array([1, 2, 3])
    with pytest.raises(RuntimeError):
        nearest = tree.get_nearest(target)