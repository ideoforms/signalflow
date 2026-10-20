import signalflow as sf
import numpy as np
import pytest
import weakref
import gc
from . import graph


class Gain(sf.Node):
    def __init__(self, input=0, gain=1.0):
        super().__init__()
        self.create_input("input", input)
        self.create_input("gain", gain)

    def process(self, num_frames):
        self.output_buffer[:, :num_frames] = (self.input.output_buffer[:, :num_frames] *
                                              self.gain.output_buffer[:, :num_frames])


class OnePole(sf.Node):
    """
    One-pole low-pass filter, with per-channel state stored in Python.
    """
    def __init__(self, input=0, coefficient=0.5):
        super().__init__()
        self.create_input("input", input)
        self.create_input("coefficient", coefficient)
        self.history = np.zeros(32)

    def process(self, num_frames):
        for channel in range(self.num_output_channels):
            input = self.input.output_buffer[channel]
            coefficient = self.coefficient.output_buffer[channel]
            output = self.output_buffer[channel]
            value = self.history[channel]
            for frame in range(num_frames):
                value = (1 - coefficient[frame]) * input[frame] + coefficient[frame] * value
                output[frame] = value
            self.history[channel] = value


class Ones(sf.Node):
    def process(self, num_frames):
        self.output_buffer[:, :num_frames] = 1


def test_python_node_process(graph):
    node = Gain(2.0, 3.0)
    assert isinstance(node, sf.Node)
    assert node.name == "Gain"
    graph.render_subgraph(node)
    assert np.all(node.output_buffer[0] == 6.0)


def test_python_node_inputs(graph):
    node = Gain(2.0, 3.0)
    assert sorted(node.inputs.keys()) == ["gain", "input"]
    node.gain = 0.5
    graph.render_subgraph(node)
    assert np.all(node.output_buffer[0] == 1.0)

    node.gain = sf.Constant(4.0)
    graph.render_subgraph(node, reset=True)
    assert np.all(node.output_buffer[0] == 8.0)

    with pytest.raises(RuntimeError):
        node.create_input("gain", 1.0)


def test_python_node_attributes(graph):
    class Stateful(sf.Node):
        def __init__(self):
            self.set_before_init = 1
            super().__init__()
            self.count = 0

        def process(self, num_frames):
            self.count += 1

    node = Stateful()
    assert node.set_before_init == 1
    graph.render_subgraph(node)
    graph.render_subgraph(node, reset=True)
    assert node.count == 2
    assert not hasattr(node, "nonexistent")
    with pytest.raises(AttributeError):
        node.nonexistent


def test_python_node_missing_super_init(graph):
    class NoSuperInit(sf.Node):
        def __init__(self):
            self.counter = 1

    with pytest.raises(TypeError, match=r"NoSuperInit.__init__\(\) must call super\(\).__init__\(\)"):
        NoSuperInit()


def test_python_node_methods_before_super_init(graph):
    class CreateInputBeforeInit(sf.Node):
        def __init__(self):
            self.create_input("input", 0)
            super().__init__()

    class SetChannelsBeforeInit(sf.Node):
        def __init__(self):
            self.set_channels(1, 1)
            super().__init__()

    with pytest.raises(TypeError, match=r"create_input\(\) cannot be called before super\(\).__init__\(\)"):
        CreateInputBeforeInit()
    with pytest.raises(TypeError, match=r"set_channels\(\) cannot be called before super\(\).__init__\(\)"):
        SetChannelsBeforeInit()


def test_python_node_properties_before_super_init(graph):
    class SetCppPropertyBeforeInit(sf.Node):
        def __init__(self):
            self.value = 1
            super().__init__()

    class SetPythonPropertyBeforeInit(sf.Node):
        def __init__(self):
            self.level = 2
            super().__init__()

        @property
        def level(self):
            return self._level

        @level.setter
        def level(self, level):
            self._level = level

    with pytest.raises(TypeError, match=r"Cannot set property 'value' before super\(\).__init__\(\)"):
        SetCppPropertyBeforeInit()
    assert SetPythonPropertyBeforeInit().level == 2


def test_python_node_stateful(graph):
    node = OnePole(1.0, 0.5)
    graph.render_subgraph(node)
    assert node.output_buffer[0][0] == pytest.approx(0.5)
    assert node.output_buffer[0][1] == pytest.approx(0.75)
    assert node.output_buffer[0][-1] == pytest.approx(1.0)


def test_python_node_operators(graph):
    node = Gain(1.0, 2.0) * 10 + 1
    graph.render_subgraph(node)
    assert np.all(node.output_buffer[0] == 21.0)

    node = Gain(sf.SineOscillator(440), 0.5)
    graph.render_subgraph(node)
    assert np.max(np.abs(node.output_buffer[0])) == pytest.approx(0.5, abs=0.01)


def test_python_node_multichannel(graph):
    node = Gain([1, 2, 3], 2.0)
    assert node.num_output_channels == 3
    graph.render_subgraph(node)
    assert list(node.output_buffer[:3, 0]) == [2, 4, 6]


def test_python_node_set_channels(graph):
    class Stereo(sf.Node):
        def __init__(self):
            super().__init__()
            self.set_channels(1, 2)

        def process(self, num_frames):
            self.output_buffer[0, :num_frames] = -1
            self.output_buffer[1, :num_frames] = 1

    node = Stereo()
    assert node.num_output_channels == 2
    assert not node.matches_input_channels
    graph.render_subgraph(node)
    assert np.all(node.output_buffer[0] == -1)
    assert np.all(node.output_buffer[1] == 1)


def test_python_node_render_to_buffer(graph):
    # More frames than the node's output buffer length, rendered in chunks
    num_frames = graph.output_buffer_size * 2 + 100
    buffer = graph.render_subgraph_to_new_buffer(Gain(1.0, 2.0), num_frames)
    assert buffer.num_frames == num_frames
    assert np.all(buffer.data[0] == 2.0)


def test_python_node_exception(graph):
    class Faulty(sf.Node):
        def process(self, num_frames):
            raise ValueError("Faulty node")

    with pytest.raises(ValueError):
        graph.render_subgraph(Faulty())


def test_python_node_no_process(graph):
    class Empty(sf.Node):
        pass

    node = Empty()
    node.output_buffer[0][0] = 1.0
    graph.render_subgraph(node)
    assert node.output_buffer[0][0] == 1.0


def test_python_node_plain_node_unchanged(graph):
    node = sf.Node()
    assert type(node) is sf.Node
    with pytest.raises(RuntimeError):
        node.create_input("input", 0)
    with pytest.raises(AttributeError):
        node.nonexistent = 1


def test_python_node_lifetime(graph):
    # A node that is played remains alive (with its Python state) even when
    # no Python references remain.
    node = Ones()
    node_ref = weakref.ref(node)
    graph.play(node)
    del node
    gc.collect()
    Ones()
    gc.collect()
    graph.render()
    assert node_ref() is not None
    assert graph.output.output_buffer[0][0] == 1.0

    # Once it is no longer referenced by either C++ or Python, it is released
    # when the next Python node is created.
    graph.stop(node_ref())
    graph.render()
    Ones()
    gc.collect()
    assert node_ref() is None
