import numpy as np
import pytest
from signalflow import KDTree, NearestNeighbour, Buffer
from . import graph

def test_kdtree(graph):
    # KDTree: Single nearest neighbour
    corpus = np.random.uniform(0, 10, [1024, 2])
    tree = KDTree(corpus)
    target = np.array([1, 5])
    nearest = tree.get_nearest(target)
    assert len(nearest) == 1
    nearest = nearest[0]

    # Check that the nearest value found is sufficiently near to the target
    assert np.all(np.isclose(target, nearest.coordinate, atol=1.0))

    # Check that the nearest value found is the same as the brute force calculation
    actual_nearest_index = np.argmin(np.linalg.norm(corpus - target, axis=1))
    assert actual_nearest_index == nearest.index

    # Check that nearest.index points to the item in the corpus identical to nearest.coordinate
    # (subject to rounding error)
    assert np.all(np.isclose(corpus[nearest.index], nearest.coordinate, atol=1e-6))

    # Check that nearest.distance calculates the correct distance
    # (subject to rounding error)
    assert nearest.distance == pytest.approx(np.linalg.norm(corpus[nearest.index] - target), abs=1e-6)

    # Confirm that the NearestNeighbour node returns the same value,
    # Constructing a buffer storing the corpus coordinates
    buffer = Buffer(corpus.T)
    nearest_neighbour = NearestNeighbour(buffer, target=target)
    graph.render_subgraph(nearest_neighbour)
    assert nearest_neighbour.output_buffer[0][0] == nearest.index

def test_kdtree_validation():
    corpus = np.random.uniform(0, 10, [1024, 2])
    tree = KDTree(corpus)
    target = np.array([1, 2, 3])
    with pytest.raises(RuntimeError):
        nearest = tree.get_nearest(target)

def test_kdtree_n_nearest(graph):
    # KDTree: Multiple nearest neighbours

    # Create a simple corpus with known positions
    corpus = np.array([
        [0.0, 0.0],
        [1.0, 0.0],
        [0.0, 1.0],
        [1.0, 1.0],
        [2.0, 0.0],
        [0.0, 2.0],
        [2.0, 2.0],
        [3.0, 3.0],
        [4.0, 4.0],
        [5.0, 5.0]
    ])
    
    # Test finding 1 nearest neighbor (default behavior)
    tree1 = KDTree(corpus, 1)
    target = np.array([2.1, 0.1])
    matches1 = tree1.get_nearest(target)
    assert len(matches1) == 1
    assert matches1[0].index == 4
    
    # Test finding 3 nearest neighbors
    tree3 = KDTree(corpus, 3)
    matches3 = tree3.get_nearest(target)
    assert len(matches3) == 3
    
    # Verify results are sorted by distance (closest first)
    for i in range(len(matches3) - 1):
        assert matches3[i].distance <= matches3[i + 1].distance
    
    # Calculate expected distances manually
    distances = np.linalg.norm(corpus - target, axis=1)
    expected_indices = np.argsort(distances)[:3]
    
    # Verify the 3 nearest are correct
    result_indices = [m.index for m in matches3]
    assert set(result_indices) == set(expected_indices)
    
    # Verify distances are correct
    for match in matches3:
        expected_distance = np.linalg.norm(corpus[match.index] - target)
        assert match.distance == pytest.approx(expected_distance, abs=1e-6)
    
    # Test finding 5 nearest neighbors
    tree5 = KDTree(corpus, 5)
    matches5 = tree5.get_nearest(target)
    assert len(matches5) == 5
    
    # Verify sorted order
    for i in range(len(matches5) - 1):
        assert matches5[i].distance <= matches5[i + 1].distance
    
    # Verify first match is the same as finding just 1
    assert matches5[0].index == matches1[0].index
    assert matches5[0].distance == pytest.approx(matches1[0].distance, abs=1e-6)
    
    # Test requesting more neighbors than exist in corpus
    tree_all = KDTree(corpus, 15)
    matches_all = tree_all.get_nearest(target)
    assert len(matches_all) == len(corpus)  # Should return all points
    
    # Verify all results are sorted
    for i in range(len(matches_all) - 1):
        assert matches_all[i].distance <= matches_all[i + 1].distance

def test_kdtree_high_dimensional():
    # KDTree: High-dimensional space
    dim = 10
    num_points = 50
    corpus = np.random.uniform(0, 10, [num_points, dim])
    tree = KDTree(corpus)
    
    target = np.random.uniform(0, 10, [dim])
    nearest = tree.get_nearest(target)
    assert len(nearest) == 1
    nearest = nearest[0]
    
    # Check that the nearest value found is the same as the brute force calculation
    distances = np.linalg.norm(corpus - target, axis=1)
    actual_nearest_index = np.argmin(distances)
    
    assert nearest.index == actual_nearest_index
    assert nearest.distance == pytest.approx(distances[actual_nearest_index], abs=1e-6)
    assert np.all(np.isclose(corpus[nearest.index], nearest.coordinate, atol=1e-6))

