from libsignal import Constant
import numpy as np

DEFAULT_BUFFER_LENGTH = 1024

def process_tree(node):
    for input in node.inputs.values():
        input.process(DEFAULT_BUFFER_LENGTH)
    node.process(DEFAULT_BUFFER_LENGTH)

def test_add():
    a = Constant(1)
    b = Constant(2)

    c = a + b
    process_tree(c)
    assert np.all(c.output_buffer == 3)

    c = a + b + 2
    process_tree(c)
    assert np.all(c.output_buffer == 5)

    c = a + 1
    process_tree(c)
    assert np.all(c.output_buffer == 2)

    c = a + 1.5
    process_tree(c)
    assert np.all(c.output_buffer == 2.5)

    c = 3 + a
    process_tree(c)
    assert np.all(c.output_buffer == 4)

    c = 3.5 + a
    process_tree(c)
    assert np.all(c.output_buffer == 4.5)

def test_subtract():
    a = Constant(1)
    b = Constant(2)

    c = a - b
    process_tree(c)
    assert np.all(c.output_buffer == -1)

    c = a - b - 2
    process_tree(c)
    assert np.all(c.output_buffer == -3)

    c = a - 1
    process_tree(c)
    assert np.all(c.output_buffer == 0)

    c = a - 1.5
    process_tree(c)
    assert np.all(c.output_buffer == -0.5)

    c = 3 - a
    process_tree(c)
    assert np.all(c.output_buffer == 2)

    c = 3.5 - a
    process_tree(c)
    assert np.all(c.output_buffer == 2.5)

def test_multiply():
    a = Constant(2)
    b = Constant(3)

    c = a * b
    process_tree(c)
    assert np.all(c.output_buffer == 6)

    c = a * b * 2
    process_tree(c)
    assert np.all(c.output_buffer == 12)

    c = a * 2
    process_tree(c)
    assert np.all(c.output_buffer == 4)

    c = a * 1.5
    process_tree(c)
    assert np.all(c.output_buffer == 3)

    c = 3 * a
    process_tree(c)
    assert np.all(c.output_buffer == 6)

    c = 3.5 * a
    process_tree(c)
    assert np.all(c.output_buffer == 7)

def test_divide():
    a = Constant(1)
    b = Constant(2)

    c = a / b
    process_tree(c)
    assert np.all(c.output_buffer == 0.5)

    c = a / b / 2
    process_tree(c)
    assert np.all(c.output_buffer == 0.25)

    c = a / 2
    process_tree(c)
    assert np.all(c.output_buffer == 0.5)

    c = a / 2.5
    process_tree(c)
    assert np.all(c.output_buffer == 0.4)

    c = 3 / b
    process_tree(c)
    assert np.all(c.output_buffer == 1.5)

    c = 3.5 / b
    process_tree(c)
    assert np.all(c.output_buffer == 1.75)

assert True
