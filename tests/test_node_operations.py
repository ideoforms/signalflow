from libsignal import Constant
import numpy as np

from . import process_tree

def test_add():
    a = Constant(1)
    b = Constant(2)

    c = a + b
    process_tree(c)
    assert np.all(c.output_buffer[0] == 3)

    c = a + b + 2
    process_tree(c)
    assert np.all(c.output_buffer[0] == 5)

    c = a + 1
    process_tree(c)
    assert np.all(c.output_buffer[0] == 2)

    c = a + 1.5
    process_tree(c)
    assert np.all(c.output_buffer[0] == 2.5)

    c = 3 + a
    process_tree(c)
    assert np.all(c.output_buffer[0] == 4)

    c = 3.5 + a
    process_tree(c)
    assert np.all(c.output_buffer[0] == 4.5)

def test_subtract():
    a = Constant(1)
    b = Constant(2)

    c = a - b
    process_tree(c)
    assert np.all(c.output_buffer[0] == -1)

    c = a - b - 2
    process_tree(c)
    assert np.all(c.output_buffer[0] == -3)

    c = a - 1
    process_tree(c)
    assert np.all(c.output_buffer[0] == 0)

    c = a - 1.5
    process_tree(c)
    assert np.all(c.output_buffer[0] == -0.5)

    c = 3 - a
    process_tree(c)
    assert np.all(c.output_buffer[0] == 2)

    c = 3.5 - a
    process_tree(c)
    assert np.all(c.output_buffer[0] == 2.5)

def test_multiply():
    a = Constant(2)
    b = Constant(3)

    c = a * b
    process_tree(c)
    assert np.all(c.output_buffer[0] == 6)

    c = a * b * 2
    process_tree(c)
    assert np.all(c.output_buffer[0] == 12)

    c = a * 2
    process_tree(c)
    assert np.all(c.output_buffer[0] == 4)

    c = a * 1.5
    process_tree(c)
    assert np.all(c.output_buffer[0] == 3)

    c = 3 * a
    process_tree(c)
    assert np.all(c.output_buffer[0] == 6)

    c = 3.5 * a
    process_tree(c)
    assert np.all(c.output_buffer[0] == 7)

def test_divide():
    a = Constant(1)
    b = Constant(2)

    c = a / b
    process_tree(c)
    assert np.all(c.output_buffer[0] == 0.5)

    c = a / b / 2
    process_tree(c)
    assert np.all(c.output_buffer[0] == 0.25)

    c = a / 2
    process_tree(c)
    assert np.all(c.output_buffer[0] == 0.5)

    c = a / 2.5
    process_tree(c)
    assert np.all(c.output_buffer[0] == 0.4)

    c = 3 / b
    process_tree(c)
    assert np.all(c.output_buffer[0] == 1.5)

    c = 3.5 / b
    process_tree(c)
    assert np.all(c.output_buffer[0] == 1.75)
