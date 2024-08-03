"""

    SignalFlow
    ----------

    A framework for audio DSP.
    
"""
from __future__ import annotations
import numpy
import typing
import typing_extensions
__all__ = ['ADSREnvelope', 'ASREnvelope', 'Abs', 'Add', 'AllpassDelay', 'AmplitudeToDecibels', 'AudioGraph', 'AudioGraphConfig', 'AudioIOException', 'AudioIn', 'AudioOut', 'AudioOut_Abstract', 'AudioOut_Dummy', 'AzimuthPanner', 'BeatCutter', 'BiquadFilter', 'Buffer', 'Buffer2D', 'BufferLooper', 'BufferPlayer', 'BufferRecorder', 'CPUUsageAboveLimitException', 'ChannelArray', 'ChannelCrossfade', 'ChannelMixer', 'ChannelPanner', 'ChannelSelect', 'Clip', 'ClockDivider', 'CombDelay', 'Compressor', 'Constant', 'Cos', 'Counter', 'CrossCorrelate', 'DCFilter', 'DecibelsToAmplitude', 'DetectSilence', 'DeviceNotFoundException', 'Divide', 'EQ', 'Envelope', 'EnvelopeBuffer', 'Equal', 'Euclidean', 'FFT', 'FFTContinuousPhaseVocoder', 'FFTContrast', 'FFTConvolve', 'FFTFindPeaks', 'FFTLPF', 'FFTMagnitudePhaseArray', 'FFTNode', 'FFTNoiseGate', 'FFTOpNode', 'FFTPhaseVocoder', 'FFTRandomPhase', 'FFTTonality', 'FFTTransform', 'FeedbackBufferReader', 'FeedbackBufferWriter', 'FlipFlop', 'Fold', 'FrequencyToMidiNote', 'Gate', 'Granulator', 'GraphAlreadyCreatedException', 'GraphNotCreatedException', 'GreaterThan', 'GreaterThanOrEqual', 'IFFT', 'If', 'Impulse', 'ImpulseSequence', 'Index', 'InvalidChannelCountException', 'KDTree', 'KDTreeMatch', 'LFO', 'Latch', 'LessThan', 'LessThanOrEqual', 'Line', 'Logistic', 'Maximiser', 'MidiNoteToFrequency', 'Modulo', 'MoogVCF', 'MouseDown', 'MouseX', 'MouseY', 'Multiply', 'NearestNeighbour', 'Node', 'NodeAlreadyPlayingException', 'NodeNotPlayingException', 'NodeRegistry', 'NotEqual', 'OneTapDelay', 'OnsetDetector', 'Patch', 'PatchFinishedPlaybackException', 'PatchRegistry', 'PatchSpec', 'PinkNoise', 'Pow', 'RMS', 'RandomBrownian', 'RandomChoice', 'RandomCoin', 'RandomExponential', 'RandomExponentialDist', 'RandomGaussian', 'RandomImpulse', 'RandomImpulseSequence', 'RandomUniform', 'RectangularEnvelope', 'Resample', 'Round', 'RoundToScale', 'SIGNALFLOW_DEFAULT_BLOCK_SIZE', 'SIGNALFLOW_DEFAULT_FFT_HOP_SIZE', 'SIGNALFLOW_DEFAULT_FFT_SIZE', 'SIGNALFLOW_DEFAULT_SAMPLE_RATE', 'SIGNALFLOW_DEFAULT_TRIGGER', 'SIGNALFLOW_EVENT_DISTRIBUTION_POISSON', 'SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM', 'SIGNALFLOW_FILTER_TYPE_BAND_PASS', 'SIGNALFLOW_FILTER_TYPE_HIGH_PASS', 'SIGNALFLOW_FILTER_TYPE_HIGH_SHELF', 'SIGNALFLOW_FILTER_TYPE_LOW_PASS', 'SIGNALFLOW_FILTER_TYPE_LOW_SHELF', 'SIGNALFLOW_FILTER_TYPE_NOTCH', 'SIGNALFLOW_FILTER_TYPE_PEAK', 'SIGNALFLOW_INTERPOLATION_MODE_COSINE', 'SIGNALFLOW_INTERPOLATION_MODE_LINEAR', 'SIGNALFLOW_INTERPOLATION_MODE_NONE', 'SIGNALFLOW_MAX_CHANNELS', 'SIGNALFLOW_MAX_FFT_SIZE', 'SIGNALFLOW_NODE_BUFFER_SIZE', 'SIGNALFLOW_NODE_STATE_ACTIVE', 'SIGNALFLOW_NODE_STATE_STOPPED', 'SIGNALFLOW_PATCH_STATE_ACTIVE', 'SIGNALFLOW_PATCH_STATE_STOPPED', 'SVFilter', 'SampleAndHold', 'SawLFO', 'SawOscillator', 'ScaleLinExp', 'ScaleLinLin', 'SegmentPlayer', 'SegmentedGranulator', 'Sequence', 'Sin', 'SineLFO', 'SineOscillator', 'Smooth', 'SpatialEnvironment', 'SpatialPanner', 'SpatialSpeaker', 'SquareLFO', 'SquareOscillator', 'Squiz', 'StereoBalance', 'StereoPanner', 'StereoWidth', 'StochasticNode', 'Stutter', 'Subtract', 'Sum', 'Tan', 'Tanh', 'TimeShift', 'TriangleLFO', 'TriangleOscillator', 'VampAnalysis', 'WaveShaper', 'WaveShaperBuffer', 'Wavetable', 'Wavetable2D', 'WetDry', 'WhiteNoise', 'Wrap', 'amplitude_to_db', 'clip', 'db_to_amplitude', 'fold', 'frequency_to_midi_note', 'midi_note_to_frequency', 'random_exponential', 'random_seed', 'random_uniform', 'save_block_to_text_file', 'save_block_to_wav_file', 'scale_exp_lin', 'scale_lin_exp', 'scale_lin_lin', 'signalflow_event_distribution_t', 'signalflow_filter_type_t', 'signalflow_interpolation_mode_t', 'signalflow_node_state_t', 'signalflow_patch_state_t', 'wrap']
class ADSREnvelope(Node):
    """
    Attack-decay-sustain-release envelope. Sustain portion is held until gate is zero.
    """
    def __init__(self, attack: Node = 0.1, decay: Node = 0.1, sustain: Node = 0.5, release: Node = 0.1, gate: Node = 0) -> None:
        ...
class ASREnvelope(Node):
    """
    Attack-sustain-release envelope.
    """
    def __init__(self, attack: Node = 0.1, sustain: Node = 0.5, release: Node = 0.1, curve: Node = 1.0, clock: Node = None) -> None:
        ...
class Abs(Node):
    """
    Outputs the absolute value of a, per sample. Can also be written as abs(a)
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class Add(Node):
    """
    Add each sample of a to each sample of b. Can also be written as a + b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class AllpassDelay(Node):
    """
    All-pass delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.
    """
    def __init__(self, input: Node = 0.0, delay_time: Node = 0.1, feedback: Node = 0.5, max_delay_time: float = 0.5) -> None:
        ...
class AmplitudeToDecibels(Node):
    """
    Map a linear amplitude value to decibels.
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class AudioGraph:
    """
    The global audio signal processing graph
    """
    output: Node
    @staticmethod
    def get_shared_graph() -> AudioGraph:
        ...
    @typing.overload
    def __init__(self, config: AudioGraphConfig = None, output_device: Node = None, start: bool = True) -> None:
        ...
    @typing.overload
    def __init__(self, config: AudioGraphConfig = None, output_device: str = '', start: bool = True) -> None:
        ...
    def add_node(self, node: Node) -> Node:
        """
        Add `node` to the graph so that it is processed in future blocks, without connecting it to the graph's output. Useful for non-playback nodes (e.g. BufferRecorder).
        """
    def clear(self) -> None:
        """
        Remove all Node and Patches objects currently in the processing graph.
        """
    def destroy(self) -> None:
        """
        Clear the AudioGraph and deallocate its memory, ready to create a new AudioGraph.
        """
    @typing.overload
    def play(self, node: Node) -> None:
        """
        Begin playback of `node` (by connecting it to the output of the graph)
        """
    @typing.overload
    def play(self, patch: ...) -> None:
        """
        Begin playback of `patch` (by connecting it to the output of the graph)
        """
    @typing.overload
    def poll(self, frequency: float) -> None:
        """
        Begin polling the AudioGraph's status every `frequency` seconds, printing it to stdout.
        """
    @typing.overload
    def poll(self) -> None:
        """
        Begin polling the AudioGraph's status every 1.0 seconds, printing it to stdout.
        """
    def remove_node(self, node: Node) -> None:
        """
        Remove a `node` that has previously been added with `add_node()`
        """
    @typing.overload
    def render(self) -> None:
        """
        Render a single block (of `output_buffer_size` frames) of the AudioGraph's output.
        """
    @typing.overload
    def render(self, num_frames: int) -> None:
        """
        Render a specified number of samples of the AudioGraph's output.
        """
    def render_subgraph(self, node: Node, num_frames: int = 0, reset: bool = False) -> None:
        """
        Recursively render the nodes in the tree starting at `node`. If `reset` is true, call `reset_subgraph` first.
        """
    def render_to_buffer(self, buffer: ...) -> None:
        """
        Render the graph's output to the specified buffer, for the same number of frames as the buffer's length.
        """
    def render_to_new_buffer(self, num_frames: int) -> ...:
        """
        Render the graph's output for the specified number of frames, and return the resultant buffer.
        """
    def replace(self, node: Node, other: Node) -> None:
        """
        Replace `node` in the graph's output with `other`.
        """
    def reset_subgraph(self, arg0: Node) -> None:
        """
        Reset the `played` status of nodes in the tree starting at `node`.
        """
    def show_structure(self) -> None:
        """
        Print the AudioGraph's node connectivity structure to stdout.
        """
    def start(self) -> None:
        """
        Start the AudioGraph processing.
        """
    def start_recording(self, filename: str = '', num_channels: int = 0) -> None:
        """
        Start recording the graph's output to an audio file, with the same number of channels as the AudioGraph or `num_channels` if specified.
        """
    @typing.overload
    def stop(self) -> None:
        """
        Stop the AudioGraph processing.
        """
    @typing.overload
    def stop(self, node: Node) -> None:
        """
        Stop playback of `node` (by disconnecting it from the output of the graph)
        """
    @typing.overload
    def stop(self, patch: ...) -> None:
        """
        Stop playback of `patch]` (by disconnecting it from the output of the graph)
        """
    def stop_recording(self) -> None:
        """
        Stop recording the graph's output.
        """
    @typing.overload
    def wait(self) -> None:
        ...
    @typing.overload
    def wait(self, seconds: float) -> None:
        ...
    @property
    def config(self) -> AudioGraphConfig:
        """
        AudioGraphConfig: Get the current audio graph configuration.
        """
    @property
    def cpu_usage(self) -> float:
        """
        float: Get the current CPU usage (from 0.0 to 1.0).
        """
    @property
    def memory_usage(self) -> int:
        """
        int: Get the current memory usage, in bytes.
        """
    @property
    def node_count(self) -> int:
        """
        int: Get the number of nodes currently active in the graph.
        """
    @property
    def num_output_channels(self) -> int:
        """
        int: Get the number of output channels available in the graph.
        """
    @property
    def output_backend_names(self) -> list[str]:
        """
        list[str]: List the available output backend names.
        """
    @property
    def output_buffer_size(self) -> int:
        """
        int: Get the current output buffer size, in frames.
        """
    @property
    def output_device_names(self) -> list[str]:
        """
        list[str]: List the available output device names.
        """
    @property
    def outputs(self) -> list[Node]:
        """
        int: Get the list of Node objects currently connected to the graph's output.
        """
    @property
    def patch_count(self) -> int:
        """
        int: Get the number of patches currently active in the graph.
        """
    @property
    def sample_rate(self) -> int:
        """
        int: Get/set the graph's sample rate.
        """
    @sample_rate.setter
    def sample_rate(self, arg1: int) -> None:
        ...
    @property
    def status(self) -> str:
        """
        int: Get a text representation of the AudioGraph's status (node count, patch count, CPU usage).
        """
    @property
    def structure(self) -> str:
        """
        int: Get a text representation of the AudioGraph's node connectivity structure.
        """
class AudioGraphConfig:
    """
    Configuration options for the AudioGraph
    """
    cpu_usage_limit: float
    input_buffer_size: int
    input_device_name: str
    output_backend_name: str
    output_buffer_size: int
    output_device_name: str
    sample_rate: int
    def __init__(self) -> None:
        ...
    def print(self) -> None:
        """
        Print the AudioGraphConfig to stdout
        """
class AudioIOException(Exception):
    pass
class AudioIn(Node):
    """
    Audio input
    """
    def __init__(self) -> None:
        ...
class AudioOut(Node):
    """
    Audio output
    """
    def __init__(self, backend_name: str = '', device_name: str = '', sample_rate: int = 0, buffer_size: int = 0) -> None:
        ...
class AudioOut_Abstract(Node):
    """
    Abstract audio output
    """
class AudioOut_Dummy(Node):
    """
    Dummy audio output for offline processing
    """
    def __init__(self, num_channels: int = 2, buffer_size: int = 256) -> None:
        ...
class AzimuthPanner(Node):
    """
    Pan input around an equally-spaced ring of `num_channels` speakers. `pan` is the pan position from -1..+1, where 0 = centre front. `width` is the source's width, where 1.0 spans exactly between an adjacent pair of channels.
    """
    def __init__(self, num_channels: int = 2, input: Node = 0, pan: Node = 0.0, width: Node = 1.0) -> None:
        ...
class BeatCutter(Node):
    """
    Cuts a buffer into segment_count segments, and stutters/jumps with the given probabilities.
    """
    def __init__(self, buffer: ... = None, segment_count: int = 8, stutter_probability: Node = 0.0, stutter_count: Node = 1, jump_probability: Node = 0.0, duty_cycle: Node = 1.0, rate: Node = 1.0, segment_rate: Node = 1.0) -> None:
        ...
class BiquadFilter(Node):
    """
    Biquad filter. filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. Not recommended for real-time modulation; for this, use SVFilter.
    """
    @typing.overload
    def __init__(self, input: Node = 0.0, filter_type: signalflow_filter_type_t = ..., cutoff: Node = 440, resonance: Node = 0.0, peak_gain: Node = 0.0) -> None:
        ...
    @typing.overload
    def __init__(self, input: Node, filter_type: str, cutoff: Node = 440, resonance: Node = 0.0, peak_gain: Node = 0.0) -> None:
        ...
class Buffer:
    """
    A buffer of audio samples, containing one or more channels.
    """
    def __add__(self: typing_extensions.Buffer, value: float) -> typing_extensions.Buffer:
        """
        Returns a new Buffer containing the samples in `self` added to `value`.
        """
    def __div__(self: typing_extensions.Buffer, value: float) -> typing_extensions.Buffer:
        """
        Returns a new Buffer containing the samples in `self` divided by `value`.
        """
    def __getitem__(self: typing_extensions.Buffer, arg0: int) -> typing_extensions.Buffer:
        ...
    @typing.overload
    def __init__(self: typing_extensions.Buffer) -> None:
        """
        Create a null Buffer with no memory allocated.
        """
    @typing.overload
    def __init__(self: typing_extensions.Buffer, filename: str) -> None:
        """
        Load a Buffer from an audio file.
        """
    @typing.overload
    def __init__(self: typing_extensions.Buffer, num_channels: int, num_frames: int) -> None:
        """
        Allocate a buffer with `num_channels` channels and `num_frames` frames.
        """
    @typing.overload
    def __init__(self: typing_extensions.Buffer, num_channels: int, num_frames: int, data: list[list[float]]) -> None:
        """
        Allocate a buffer with `num_channels` channels and `num_frames` frames, containing the floating-point samples in `data`.
        """
    @typing.overload
    def __init__(self: typing_extensions.Buffer, arg0: list[list[float]]) -> None:
        """
        Allocate a buffer with `num_channels` channels and `num_frames` frames, containing the floating-point samples in `data`.
        """
    @typing.overload
    def __init__(self: typing_extensions.Buffer, data: list[float]) -> None:
        """
        Allocate a buffer containing the floating-point samples in `data`.
        """
    @typing.overload
    def __init__(self: typing_extensions.Buffer, function: typing.Callable[[float], float]) -> None:
        """
        Allocate a buffer filled with the output of the function `function`.
        """
    @typing.overload
    def __init__(self: typing_extensions.Buffer, num_frames: int, function: typing.Callable[[float], float]) -> None:
        """
        Allocate a mono buffer with `num_frames` frames, filled with the output of the function `function`.
        """
    @typing.overload
    def __init__(self: typing_extensions.Buffer, num_channels: int, num_frames: int, function: typing.Callable[[float], float]) -> None:
        """
        Allocate a buffer with `num_channels` channels and `num_frames` frames, filled with the output of the function `function`.
        """
    def __len__(self: typing_extensions.Buffer) -> int:
        """
        Returns the length of the buffer `self`, in frames.
        """
    def __mul__(self: typing_extensions.Buffer, value: float) -> typing_extensions.Buffer:
        """
        Returns a new Buffer containing the samples in `self` multiplied by `value`.
        """
    def __radd__(self: typing_extensions.Buffer, value_: float) -> typing_extensions.Buffer:
        """
        Returns a new Buffer containing the samples in `self` added to `value`.
        """
    def __rmul__(self: typing_extensions.Buffer, value_: float) -> typing_extensions.Buffer:
        """
        Returns a new Buffer containing the samples in `self` multiplied by `value`.
        """
    def __str__(self: typing_extensions.Buffer) -> str:
        ...
    def __sub__(self: typing_extensions.Buffer, value: float) -> typing_extensions.Buffer:
        """
        Returns a new Buffer containing the samples in `self` subtracted by `value`.
        """
    @typing.overload
    def fill(self: typing_extensions.Buffer, sample: float) -> None:
        ...
    @typing.overload
    def fill(self: typing_extensions.Buffer, function: typing.Callable[[float], float]) -> None:
        ...
    def get(self: typing_extensions.Buffer, channel: int, frame: float) -> float:
        ...
    def get_frame(self: typing_extensions.Buffer, channel: int, frame: float) -> float:
        ...
    def load(self: typing_extensions.Buffer, filename: str) -> None:
        ...
    def save(self: typing_extensions.Buffer, filename: str) -> None:
        ...
    def set(self: typing_extensions.Buffer, channel: int, frame: int, value: float) -> bool:
        ...
    def split(self: typing_extensions.Buffer, num_frames_per_part: int) -> list[typing_extensions.Buffer]:
        ...
    @property
    def data(self) -> numpy.ndarray[numpy.float32]:
        ...
    @property
    def duration(self) -> float:
        """
        Returns the buffer's duration, in seconds.
        """
    @property
    def filename(self) -> str:
        """
        Returns the buffer's filename, if the buffer has been loaded from/saved to file.
        """
    @property
    def frame_offsets(self) -> list[float]:
        """
        Returns a list containing all frame numbers"
        """
    @property
    def interpolation_mode(self) -> signalflow_interpolation_mode_t:
        """
        Get/set the buffer's interpolation mode.
        """
    @interpolation_mode.setter
    def interpolation_mode(self, arg1: signalflow_interpolation_mode_t) -> None:
        ...
    @property
    def num_channels(self) -> int:
        """
        Returns the number of channels in the buffer.
        """
    @property
    def num_frames(self) -> int:
        """
        Returns the number of frames the buffer.
        """
    @property
    def sample_rate(self) -> float:
        """
        Returns the buffer's sample rate.
        """
class Buffer2D(Buffer):
    """
    Two-dimensional buffer of audio samples
    """
    def __init__(self, arg0: list[typing_extensions.Buffer]) -> None:
        ...
    def get2D(self, arg0: float, arg1: float) -> float:
        ...
class BufferLooper(Node):
    """
    Read and write from a buffer concurrently, with controllable overdub.
    """
    def __init__(self, buffer: ... = None, input: Node = 0.0, feedback: Node = 0.0, loop_playback: bool = False, loop_record: bool = False) -> None:
        ...
class BufferPlayer(Node):
    """
    Plays the contents of the given buffer. start_time/end_time are in seconds. When a clock signal is received, rewinds to the start_time.
    """
    def __init__(self, buffer: ... = None, rate: Node = 1.0, loop: Node = 0, start_time: Node = None, end_time: Node = None, clock: Node = None) -> None:
        ...
class BufferRecorder(Node):
    """
    Records the input to a buffer. feedback controls overdub.
    """
    def __init__(self, buffer: ... = None, input: Node = 0.0, feedback: Node = 0.0, loop: bool = False) -> None:
        ...
class CPUUsageAboveLimitException(Exception):
    pass
class ChannelArray(Node):
    """
    Takes an array of inputs and spreads them across multiple channels of output.
    """
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, inputs: ...) -> None:
        ...
    @typing.overload
    def __init__(self, inputs: list[Node]) -> None:
        ...
    @typing.overload
    def __init__(self, inputs: list[int]) -> None:
        ...
    @typing.overload
    def __init__(self, inputs: list[float]) -> None:
        ...
class ChannelCrossfade(Node):
    """
    Given a multichannel input, crossfades between channels based on the given position within the virtual array, producing a single-channel output.
    """
    def __init__(self, input: Node = None, index: Node = None, num_output_channels: int = 1) -> None:
        ...
class ChannelMixer(Node):
    """
    Downmix a multichannel input to a lower-channel output. If num_channels is greater than one, spreads the input channels across the field. If amplitude_compensation is enabled, scale down the amplitude based on the ratio of input to output channels.
    """
    def __init__(self, num_channels: int = 1, input: Node = 0, amplitude_compensation: bool = True) -> None:
        ...
class ChannelPanner(Node):
    """
    Pan the input between a linear series of channels, where `pan` 0 = channel 0, 1 = channel 1, etc. No wrapping is applied.
    """
    def __init__(self, num_channels: int = 2, input: Node = 0, pan: Node = 0.0, width: Node = 1.0) -> None:
        ...
class ChannelSelect(Node):
    """
    Select a subset of channels from a multichannel input, starting at offset, up to a maximum of maximum, with the given step.
    """
    def __init__(self, input: Node = None, offset: int = 0, maximum: int = 0, step: int = 1) -> None:
        ...
class Clip(Node):
    """
    Clip the input to `min`/`max`.
    """
    def __init__(self, input: Node = None, min: Node = -1.0, max: Node = 1.0) -> None:
        ...
class ClockDivider(Node):
    """
    When given a `clock` input (e.g., an Impulse), divides the clock by the given `factor`. factor must be an integer greater than or equal to 1.
    """
    def __init__(self, clock: Node = 0, factor: Node = 1) -> None:
        ...
class CombDelay(Node):
    """
    Comb delay, with `feedback` between 0 and 1. `delay_time` must be less than or equal to `max_delay_time`.
    """
    def __init__(self, input: Node = 0.0, delay_time: Node = 0.1, feedback: Node = 0.5, max_delay_time: float = 0.5) -> None:
        ...
class Compressor(Node):
    """
    Dynamic range compression, with optional `sidechain` input. When the input amplitude is above `threshold`, compresses the amplitude with the given `ratio`, following the given `attack_time` and `release_time` in seconds.
    """
    def __init__(self, input: Node = 0.0, threshold: Node = 0.1, ratio: Node = 2, attack_time: Node = 0.01, release_time: Node = 0.1, sidechain: Node = None) -> None:
        ...
class Constant(Node):
    """
    Produces a constant value.
    """
    def __init__(self, value: float = 0) -> None:
        ...
class Cos(Node):
    """
    Outputs cos(a), per sample.
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class Counter(Node):
    """
    Count upwards from `min` to `max`, driven by `clock`.
    """
    def __init__(self, clock: Node = 0, min: Node = 0, max: Node = 2147483647) -> None:
        ...
class CrossCorrelate(Node):
    """
    Outputs the cross-correlation of the input signal with the given buffer. If hop_size is zero, calculates the cross-correlation every sample.
    """
    def __init__(self, input: Node = None, buffer: ... = None, hop_size: int = 0) -> None:
        ...
class DCFilter(Node):
    """
    Remove low-frequency and DC content from a signal.
    """
    def __init__(self, input: Node = 0.0) -> None:
        ...
class DecibelsToAmplitude(Node):
    """
    DecibelsToAmplitude
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class DetectSilence(Node):
    """
    Detects blocks of silence below the threshold value. Used as an auto-free node to terminate a Patch after processing is complete.
    """
    def __init__(self, input: Node = None, threshold: Node = 1e-05) -> None:
        ...
class DeviceNotFoundException(Exception):
    pass
class Divide(Node):
    """
    Divide each sample of a by each sample of b. Can also be written as a / b
    """
    def __init__(self, a: Node = 1, b: Node = 1) -> None:
        ...
class EQ(Node):
    """
    Three-band EQ.
    """
    def __init__(self, input: Node = 0.0, low_gain: Node = 1.0, mid_gain: Node = 1.0, high_gain: Node = 1.0, low_freq: Node = 500, high_freq: Node = 5000) -> None:
        ...
class Envelope(Node):
    """
    Generic envelope constructor, given an array of levels, times and curves.
    """
    def __init__(self, levels: list[Node] = [], times: list[Node] = [], curves: list[Node] = [], clock: Node = None, loop: bool = False) -> None:
        ...
class EnvelopeBuffer(Buffer):
    """
    Buffer encapsulating an audio envelope
    """
    @typing.overload
    def __init__(self, num_frames: int) -> None:
        """
        Create an envelope buffer containing the given number of samples.
        """
    @typing.overload
    def __init__(self, samples: list[float]) -> None:
        """
        Create an envelope buffer containing the specified 1D array of samples.
        """
    @typing.overload
    def __init__(self, shape: str) -> None:
        """
        Create an envelope buffer with the specified shape, one of: rectangular, triangle, hanning, linear-decay.
        """
    @typing.overload
    def __init__(self, shape: str, num_frames: int) -> None:
        """
        Create an envelope buffer with the specified shape and number of frames.
        """
    @typing.overload
    def __init__(self, function: typing.Callable[[float], float]) -> None:
        """
        Create an envelope buffer filled with the output of a given function.
        """
    @property
    def frame_offsets(self) -> list[float]:
        """
        Returns a list containing the offset in the envelope buffer for each frame, ranging over 0..1.
        """
class Equal(Node):
    """
    Compares the output of a to the output of b. Outputs 1 when equal, 0 otherwise. Can also be written as a == b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class Euclidean(Node):
    """
    Euclidean rhythm as described by Toussaint, with `sequence_length` (n) and `num_events` (k), driven by `clock`.
    """
    def __init__(self, clock: Node = 0, sequence_length: Node = 0, num_events: Node = 0) -> None:
        ...
class FFT(FFTNode):
    """
    Fast Fourier Transform. Takes a time-domain input, and generates a frequency-domain (FFT) output.
    """
    def __init__(self, input: Node = 0.0, fft_size: int = 1024, hop_size: int = 128, window_size: int = 0, do_window: bool = True) -> None:
        ...
class FFTContinuousPhaseVocoder(FFTNode):
    """
    Continuous phase vocoder. Requires an FFT* input.
    """
    def __init__(self, input: Node = None, rate: float = 1.0) -> None:
        ...
class FFTContrast(FFTOpNode):
    """
    FFT Contrast. Requires an FFT* input.
    """
    def __init__(self, input: Node = 0, contrast: Node = 1) -> None:
        ...
class FFTConvolve(FFTOpNode):
    """
    Frequency-domain convolution, using overlap-add. Useful for convolution reverb, with the input buffer containing an impulse response. Requires an FFT* input.
    """
    def __init__(self, input: Node = None, buffer: ... = None) -> None:
        ...
class FFTFindPeaks(FFTOpNode):
    """
    Find peaks in the FFT magnitude spectrum. Requires an FFT* input.
    """
    def __init__(self, input: Node = 0, prominence: Node = 1, threshold: Node = 1e-06, count: int = 64, interpolate: bool = True) -> None:
        ...
class FFTLPF(FFTOpNode):
    """
    FFT-based brick wall low pass filter. Requires an FFT* input.
    """
    def __init__(self, input: Node = 0, frequency: Node = 2000) -> None:
        ...
class FFTMagnitudePhaseArray(FFTOpNode):
    """
    Fixed mag/phase array.
    """
    def __init__(self, input: Node = 0, magnitudes: list[float] = 0, phases: list[float] = 0) -> None:
        ...
    def set_magnitudes(self, arg0: list[float]) -> None:
        ...
class FFTNode(Node):
    def get_magnitudes(self) -> list[float]:
        ...
    def get_phases(self) -> list[float]:
        ...
class FFTNoiseGate(FFTOpNode):
    """
    FFT-based noise gate. Requires an FFT* input.
    """
    def __init__(self, input: Node = 0, threshold: Node = 0.5, invert: Node = 0.0) -> None:
        ...
class FFTOpNode(FFTNode):
    pass
class FFTPhaseVocoder(FFTOpNode):
    """
    Phase vocoder. Requires an FFT* input.
    """
    def __init__(self, input: Node = None) -> None:
        ...
class FFTRandomPhase(FFTOpNode):
    """
    Randomise phase values.
    """
    def __init__(self, input: Node = 0, level: Node = 1.0) -> None:
        ...
class FFTTonality(FFTOpNode):
    """
    Tonality filter. Requires an FFT* input.
    """
    def __init__(self, input: Node = 0, level: Node = 0.5, smoothing: Node = 0.9) -> None:
        ...
class FFTTransform(FFTOpNode):
    """
    Transforms the FFT magnitude spectrum in the X axis. Requires an FFT* input.
    """
    def __init__(self, input: Node = 0, flip: Node = 0, rotate: Node = 0) -> None:
        ...
class FeedbackBufferReader(Node):
    """
    Counterpart to FeedbackBufferWriter.
    """
    def __init__(self, buffer: ... = None) -> None:
        ...
class FeedbackBufferWriter(Node):
    """
    Counterpart to FeedbackBufferReader.
    """
    def __init__(self, buffer: ... = None, input: Node = 0.0, delay_time: Node = 0.1) -> None:
        ...
class FlipFlop(Node):
    """
    Flips from 0/1 on each `clock`.
    """
    def __init__(self, clock: Node = 0) -> None:
        ...
class Fold(Node):
    """
    Fold the input beyond `min`/`max`, reflecting the excess back.
    """
    def __init__(self, input: Node = None, min: Node = -1.0, max: Node = 1.0) -> None:
        ...
class FrequencyToMidiNote(Node):
    """
    Map a frequency to a MIDI note (where 440Hz = A4 = 69), with floating-point output.
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class Gate(Node):
    """
    Outputs the input value when it is above the given `threshold`, otherwise zero.
    """
    def __init__(self, input: Node = 0.0, threshold: Node = 0.1) -> None:
        ...
class Granulator(Node):
    """
    Granulator. Generates a grain from the given buffer each time a clock signal is received, with the given duration/rate/pan parameters. The input buffer can be mono or stereo. If `wrap` is true, grain playback can wrap around the end/start of the buffer.
    """
    def __init__(self, buffer: ... = None, clock: Node = 0, pos: Node = 0, duration: Node = 0.1, pan: Node = 0.0, rate: Node = 1.0, max_grains: Node = 2048, wrap: bool = False) -> None:
        ...
class GraphAlreadyCreatedException(Exception):
    pass
class GraphNotCreatedException(Exception):
    pass
class GreaterThan(Node):
    """
    Compares the output of a to the output of b. Outputs 1 when a > b, 0 otherwise. Can also be written as a > b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class GreaterThanOrEqual(Node):
    """
    Compares the output of a to the output of b. Outputs 1 when a >= b, 0 otherwise. Can also be written as a >= b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class IFFT(FFTOpNode):
    """
    Inverse Fast Fourier Transform. Requires an FFT* input, generates a time-domain output.
    """
    def __init__(self, input: Node = None, do_window: bool = False) -> None:
        ...
class If(Node):
    """
    Outputs value_if_true for each non-zero value of a, value_if_false for all other values.
    """
    def __init__(self, a: Node = 0, value_if_true: Node = 0, value_if_false: Node = 0) -> None:
        ...
class Impulse(Node):
    """
    Produces a value of 1 at the given `frequency`, with output of 0 at all other times. If frequency is 0, produces a single impulse.
    """
    def __init__(self, frequency: Node = 1.0) -> None:
        ...
class ImpulseSequence(Node):
    """
    Each time a `clock` or trigger is received, outputs the next value in `sequence`. At all other times, outputs zero.
    """
    @typing.overload
    def __init__(self, sequence: list[int] = [], clock: Node = None) -> None:
        ...
    @typing.overload
    def __init__(self, sequence: str, clock: Node = None) -> None:
        ...
class Index(Node):
    """
    Outputs the value in `list` corresponding to `index`.
    """
    def __init__(self, list: list[float] = 0, index: Node = 0) -> None:
        ...
class InvalidChannelCountException(Exception):
    pass
class KDTree:
    """
    A KDTree structure
    """
    def __init__(self, data: list[list[float]] = None) -> None:
        ...
    def get_nearest(self, target: list[float]) -> KDTreeMatch:
        ...
class KDTreeMatch:
    """
    A KDTreeMatch result
    """
    @property
    def coordinate(self) -> list[float]:
        """
        The coordinate
        """
    @property
    def distance(self) -> float:
        """
        The distance
        """
    @property
    def index(self) -> int:
        """
        The index
        """
class LFO(Node):
    """
    LFO
    """
    def __init__(self, frequency: Node = 1.0, min: Node = 0.0, max: Node = 1.0, phase: Node = 0.0) -> None:
        ...
class Latch(Node):
    """
    Initially outputs 0. When a trigger is received at `set`, outputs 1. When a trigger is subsequently received at `reset`, outputs 0, until the next `set`.
    """
    def __init__(self, set: Node = 0, reset: Node = 0) -> None:
        ...
class LessThan(Node):
    """
    Compares the output of a to the output of b. Outputs 1 when a < b, 0 otherwise. Can also be written as a < b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class LessThanOrEqual(Node):
    """
    Compares the output of a to the output of b. Outputs 1 when a <= b, 0 otherwise. Can also be written as a <= b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class Line(Node):
    """
    Line segment with the given start/end values, and duration (in seconds). If loop is true, repeats indefinitely. Retriggers on a clock signal.
    """
    def __init__(self, start: Node = 0.0, end: Node = 1.0, time: Node = 1.0, loop: Node = 0, clock: Node = None) -> None:
        ...
class Logistic(Node):
    """
    Logistic noise.
    """
    def __init__(self, chaos: Node = 3.7, frequency: Node = 0.0) -> None:
        ...
class Maximiser(Node):
    """
    Gain maximiser.
    """
    def __init__(self, input: Node = 0.0, ceiling: Node = 0.5, attack_time: Node = 1.0, release_time: Node = 1.0) -> None:
        ...
class MidiNoteToFrequency(Node):
    """
    Map a MIDI note to a frequency (where 440Hz = A4 = 69), supporting floating-point input.
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class Modulo(Node):
    """
    Outputs the value of a modulo b, per sample. Supports fractional values. Can also be written as a % b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class MoogVCF(Node):
    """
    Moog ladder low-pass filter.
    """
    def __init__(self, input: Node = 0.0, cutoff: Node = 200.0, resonance: Node = 0.0) -> None:
        ...
class MouseDown(Node):
    """
    Outputs 1 if the left mouse button is down, 0 otherwise. Currently only supported on macOS.
    """
    def __init__(self, button_index: Node = 0) -> None:
        ...
class MouseX(Node):
    """
    Outputs the normalised cursor X position, from 0 to 1. Currently only supported on macOS.
    """
    def __init__(self) -> None:
        ...
class MouseY(Node):
    """
    Outputs the normalised cursor Y position, from 0 to 1. Currently only supported on macOS.
    """
    def __init__(self) -> None:
        ...
class Multiply(Node):
    """
    Multiply each sample of a by each sample of b. Can also be written as a * b
    """
    def __init__(self, a: Node = 1.0, b: Node = 1.0) -> None:
        ...
class NearestNeighbour(Node):
    """
    Nearest Neighbour.
    """
    def __init__(self, buffer: ... = None, target: Node = 0.0) -> None:
        ...
class Node:
    """
    A single signal-processing unit in the DSP graph, a node generates audio samples, optionally acting on zero or more input nodes.
    """
    @typing.overload
    def __add__(self, other: Node) -> Node:
        """
        Returns a new Node whose output is equal to `self` + `other`
        """
    @typing.overload
    def __add__(self, value: float) -> Node:
        """
        Returns a new Node whose output is equal to `self` + `value`.
        """
    def __eq__(self, other: Node) -> ...:
        """
        Returns a new Node whose output is equal to `self` == `other`
        """
    def __ge__(self, other: Node) -> ...:
        """
        Returns a new Node whose output is equal to `self` >= `other`
        """
    def __getattr__(self, input_name: str) -> Node:
        """
        Get the value of the input named `input_name`
        """
    @typing.overload
    def __getitem__(self, index: int) -> ...:
        """
        Returns a new Node whose output is equal to the `index`'th channel of `self`
        """
    @typing.overload
    def __getitem__(self, slice: slice) -> ...:
        """
        Returns a new Node whose output is equal to the channels of `self` denoted by `slice`
        """
    def __gt__(self, other: Node) -> ...:
        """
        Returns a new Node whose output is equal to `self` > `other`
        """
    def __hash__(self) -> int:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, value: int) -> None:
        """
        Creates a new Constant node which outputs a value of `value`.
        """
    @typing.overload
    def __init__(self, value: float) -> None:
        """
        Creates a new Constant node which outputs a value of `value`.
        """
    @typing.overload
    def __init__(self, arg0: list[Node]) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: ...) -> None:
        ...
    def __le__(self, other: Node) -> ...:
        """
        Returns a new Node whose output is equal to `self` <= `other`
        """
    def __lt__(self, other: Node) -> ...:
        """
        Returns a new Node whose output is equal to `self` < `other`
        """
    def __mod__(self, other: Node) -> ...:
        """
        Returns a new Node whose output is equal to `self` % `other`
        """
    @typing.overload
    def __mul__(self, other: Node) -> Node:
        """
        Returns a new Node whose output is equal to `self` * `other`
        """
    @typing.overload
    def __mul__(self, value: float) -> Node:
        """
        Returns a new Node whose output is equal to `self` * `value`
        """
    def __ne__(self, other: Node) -> ...:
        """
        Returns a new Node whose output is equal to `self` != `other`
        """
    @typing.overload
    def __pow__(self, other: Node) -> ...:
        """
        Returns a new Node whose output is equal to `self` ** `other`
        """
    @typing.overload
    def __pow__(self, value: float) -> ...:
        """
        Returns a new Node whose output is equal to `self` ** `value`
        """
    def __radd__(self, value: float) -> Node:
        """
        Returns a new Node whose output is equal to `self` + `value`
        """
    def __rmul__(self, value: float) -> Node:
        """
        Returns a new Node whose output is equal to `self` * `value`
        """
    def __rpow__(self, value: float) -> ...:
        """
        Returns a new Node whose output is equal to `self` ** `value`
        """
    def __rsub__(self, value: float) -> Node:
        """
        Returns a new Node whose output is equal to `self` - `value`
        """
    def __rtruediv__(self, other: float) -> Node:
        """
        Returns a new Node whose output is equal to `self` * `other`
        """
    @typing.overload
    def __setattr__(self, input_name: str, value: float) -> None:
        """
        Set the Node's input `input_name` to value `value`
        """
    @typing.overload
    def __setattr__(self, arg0: str, arg1: Node) -> None:
        ...
    @typing.overload
    def __sub__(self, other: Node) -> Node:
        """
        Returns a new Node whose output is equal to `self` - `other`
        """
    @typing.overload
    def __sub__(self, value: float) -> Node:
        """
        Returns a new Node whose output is equal to `self` - `value`
        """
    @typing.overload
    def __truediv__(self, other: Node) -> Node:
        """
        Returns a new Node whose output is equal to `self` * `other`
        """
    @typing.overload
    def __truediv__(self, value: float) -> Node:
        """
        Returns a new Node whose output is equal to `self` * `other`
        """
    def add_input(self, name: Node) -> None:
        """
        When instantiating a new Node object, specifies the name of a new audio-rate input
        """
    def get_input(self, name: str) -> Node:
        """
        Get the value of the input named `name`
        """
    def get_property(self, arg0: str) -> typing.Any:
        ...
    def get_value(self) -> float:
        """
        Get the node's current value. Only applicable to Constant nodes.
        """
    def play(self) -> None:
        """
        Begin playing the node by connecting it to the graph's output
        """
    @typing.overload
    def poll(self) -> None:
        """
        Print the node's last output value, once per second
        """
    @typing.overload
    def poll(self, arg0: float) -> None:
        """
        Print the node's last output value, at a frequency of `frequency`
        """
    @typing.overload
    def poll(self, arg0: float, arg1: str) -> None:
        """
        Print the node's last output value, at a frequency of `frequency`, labelled with `label`
        """
    @typing.overload
    def process(self, num_frames: int) -> None:
        """
        Generates `num_frames` of output
        """
    @typing.overload
    def process(self, buffer: ...) -> None:
        """
        Generates output to fill buffer `buffer`
        """
    def remove_input(self, name: Node) -> None:
        """
        Removes a named input from a Node object. Generally only applies to variable-input nodes.
        """
    @typing.overload
    def scale(self, from: float, to: float) -> Node:
        """
        Linearly scales the output of the node to the range `from` ... `to`
        """
    @typing.overload
    def scale(self, from: float, to: float, scale: signalflow_scale_t) -> Node:
        """
        Linearly scales the output of the node to the range `from` ... `to`, with scale `scale`
        """
    def set_buffer(self, string: str, buffer: ...) -> None:
        """
        Set the value of a node's buffer input
        """
    @typing.overload
    def set_input(self, name: str, value: float) -> None:
        """
        Set the input named `name` to `value`
        """
    @typing.overload
    def set_input(self, name: str, node: Node) -> None:
        """
        Set the input named `name` to node `node`
        """
    def set_value(self, value: float) -> None:
        """
        Set the node's current value. Only applicable to Constant nodes.
        """
    def stop(self) -> None:
        """
        Stop playing the node by disconnecting it from the graph's output
        """
    @typing.overload
    def trigger(self) -> None:
        """
        Triggers the default trigger of a node
        """
    @typing.overload
    def trigger(self, name: str) -> None:
        """
        Triggers the trigger named `name`
        """
    @typing.overload
    def trigger(self, name: str, value: float) -> None:
        """
        Triggers the trigger named `name` with value `value`
        """
    @property
    def has_variable_inputs(self) -> bool:
        """
        If true, indicates that the node can support an arbitrary number of inputs (e.g. 'Sum')
        """
    @property
    def inputs(self) -> dict[str, Node]:
        """
        Returns a dict of the node's input names and values
        """
    @property
    def is_playing(self) -> bool:
        """
        Query whether the node is currently playing
        """
    @property
    def matches_input_channels(self) -> bool:
        """
        If true, indicates that the node will output the same number of channels that it receives
        """
    @property
    def name(self) -> str:
        """
        Short human-readable identifier for the node type (e.g. 'comb-delay')
        """
    @property
    def num_input_channels(self) -> int:
        """
        The number of input channels expected by the node
        """
    @property
    def num_output_channels(self) -> int:
        """
        The number of output channels that the node will currently generate
        """
    @property
    def num_output_channels_allocated(self) -> int:
        """
        The number of channels allocated in the output buffer
        """
    @property
    def output_buffer(self) -> numpy.ndarray[numpy.float32]:
        """
        Returns the node's current output buffer, as a numpy array
        """
    @property
    def output_buffer_length(self) -> int:
        """
        The length of the node's output buffer, in frames
        """
    @property
    def outputs(self) -> list[Node]:
        """
        Returns a list of the node's output connections
        """
    @property
    def patch(self) -> ...:
        """
        Returns the patch that a node is part of, or None if the node is not part of a patch
        """
    @property
    def state(self) -> ...:
        """
        Returns the node's current playback state
        """
    @property
    def value(self) -> float:
        """
        Get/set the node's current value. Only applicable to Constant nodes.
        """
    @value.setter
    def value(self, arg1: float) -> None:
        ...
class NodeAlreadyPlayingException(Exception):
    pass
class NodeNotPlayingException(Exception):
    pass
class NodeRegistry:
    def __init__(self) -> None:
        ...
    def create(self, arg0: str) -> Node:
        ...
class NotEqual(Node):
    """
    Compares the output of a to the output of b. Outputs 0 when equal, 1 otherwise. Can also be written as a != b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class OneTapDelay(Node):
    """
    Single-tap delay line. `delay_time` must be less than or equal to `max_delay_time`.
    """
    def __init__(self, input: Node = 0.0, delay_time: Node = 0.1, max_delay_time: float = 0.5) -> None:
        ...
class OnsetDetector(Node):
    """
    Simple time-domain onset detector. Outputs an impulse when an onset is detected in the input. Maintains short-time and long-time averages. An onset is registered when the short-time average is threshold x the long-time average. min_interval is the minimum interval between onsets, in seconds.
    """
    def __init__(self, input: Node = 0.0, threshold: Node = 2.0, min_interval: Node = 0.1) -> None:
        ...
class Patch:
    """
    A Patch encapsulates a connected network of nodes, analogous to a synthesizer
    """
    auto_free: bool
    auto_free_node: Node
    output: Node
    trigger_node: Node
    @typing.overload
    def __add__(self, arg0: Node) -> Node:
        ...
    @typing.overload
    def __add__(self, arg0: float) -> Node:
        ...
    @typing.overload
    def __init__(self, arg0: ..., arg1: dict[str, Node]) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: ...) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __mul__(self, arg0: Node) -> Node:
        ...
    @typing.overload
    def __mul__(self, arg0: float) -> Node:
        ...
    def __radd__(self, arg0: float) -> Node:
        ...
    def __rmul__(self, arg0: float) -> Node:
        ...
    def __rsub__(self, arg0: float) -> Subtract:
        ...
    def __rtruediv__(self, arg0: float) -> Divide:
        ...
    @typing.overload
    def __sub__(self, arg0: Node) -> Node:
        ...
    @typing.overload
    def __sub__(self, arg0: float) -> Node:
        ...
    @typing.overload
    def __truediv__(self, arg0: Node) -> Node:
        ...
    @typing.overload
    def __truediv__(self, arg0: float) -> Node:
        ...
    @typing.overload
    def add_buffer_input(self, arg0: str) -> typing_extensions.Buffer:
        ...
    @typing.overload
    def add_buffer_input(self, arg0: str, arg1: typing_extensions.Buffer) -> typing_extensions.Buffer:
        ...
    @typing.overload
    def add_input(self, arg0: str) -> Node:
        ...
    @typing.overload
    def add_input(self, arg0: str, arg1: float) -> Node:
        ...
    @typing.overload
    def add_input(self, arg0: str, arg1: Node) -> Node:
        ...
    def add_node(self, arg0: Node) -> Node:
        ...
    def add_to_graph(self) -> None:
        ...
    def get_auto_free(self) -> bool:
        ...
    def get_auto_free_node(self) -> Node:
        ...
    def get_trigger_node(self) -> Node:
        ...
    def play(self) -> None:
        ...
    def set_auto_free(self, arg0: bool) -> None:
        ...
    def set_auto_free_node(self, arg0: Node) -> None:
        ...
    @typing.overload
    def set_input(self, arg0: str, arg1: float) -> None:
        ...
    @typing.overload
    def set_input(self, arg0: str, arg1: Node) -> None:
        ...
    @typing.overload
    def set_input(self, arg0: str, arg1: typing_extensions.Buffer) -> None:
        ...
    def set_output(self, arg0: Node) -> None:
        ...
    def set_trigger_node(self, arg0: Node) -> None:
        ...
    def stop(self) -> None:
        ...
    def to_spec(self) -> ...:
        ...
    @typing.overload
    def trigger(self) -> None:
        ...
    @typing.overload
    def trigger(self, arg0: str) -> None:
        ...
    @typing.overload
    def trigger(self, arg0: str, arg1: float) -> None:
        ...
    @property
    def graph(self) -> AudioGraph:
        ...
    @property
    def inputs(self) -> dict[str, Node]:
        ...
    @property
    def nodes(self) -> set[Node]:
        ...
    @property
    def state(self) -> ...:
        ...
class PatchFinishedPlaybackException(Exception):
    pass
class PatchRegistry:
    def __init__(self) -> None:
        ...
    def create(self, arg0: str) -> Patch:
        ...
class PatchSpec:
    @typing.overload
    def __init__(self, arg0: str) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    def from_json(self, arg0: str) -> None:
        ...
    def load(self, arg0: str) -> None:
        ...
    def print(self) -> None:
        ...
    def save(self, arg0: str) -> None:
        ...
    def to_json(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
class PinkNoise(StochasticNode):
    """
    Pink noise, with specified low/high cutoffs.
    """
    def __init__(self, low_cutoff: float = 20.0, high_cutoff: float = 20000.0, reset: Node = None) -> None:
        ...
class Pow(Node):
    """
    Outputs a to the power of b, per sample. Can also be written as a ** b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class RMS(Node):
    """
    Outputs the root-mean-squared value of the input, in buffers equal to the graph's current buffer size.
    """
    def __init__(self, input: Node = 0.0) -> None:
        ...
class RandomBrownian(StochasticNode):
    """
    Outputs Brownian noise between min/max, with a mean change of delta between samples. If a clock is passed, only generates a new value on a clock tick.
    """
    def __init__(self, min: Node = -1.0, max: Node = 1.0, delta: Node = 0.01, clock: Node = None, reset: Node = None) -> None:
        ...
class RandomChoice(StochasticNode):
    """
    Pick a random value from the given array. If a clock is passed, only picks a new value on a clock tick.
    """
    def __init__(self, values: list[float] = [], clock: Node = None, reset: Node = None) -> None:
        ...
class RandomCoin(StochasticNode):
    """
    Flip a coin with the given probability. If a clock is passed, only picks a new value on a clock tick.
    """
    def __init__(self, probability: Node = 0.5, clock: Node = None, reset: Node = None) -> None:
        ...
class RandomExponential(StochasticNode):
    """
    Generate an random exponential value between min/max. If a clock is passed, only picks a new value on a clock tick.
    """
    def __init__(self, min: Node = 0.001, max: Node = 1.0, clock: Node = None, reset: Node = None) -> None:
        ...
class RandomExponentialDist(StochasticNode):
    """
    Generate an random value following the exponential distribution. If a clock is passed, only picks a new value on a clock tick.
    """
    def __init__(self, scale: Node = 0.0, clock: Node = None, reset: Node = None) -> None:
        ...
class RandomGaussian(StochasticNode):
    """
    Generate an random Gaussian value, with given mean and sigma. If a clock is passed, only picks a new value on a clock tick.
    """
    def __init__(self, mean: Node = 0.0, sigma: Node = 0.0, clock: Node = None, reset: Node = None) -> None:
        ...
class RandomImpulse(StochasticNode):
    """
    Generate random impulses at the given frequency, with either uniform or poisson distribution.
    """
    @typing.overload
    def __init__(self, frequency: Node = 1.0, distribution: signalflow_event_distribution_t = ..., reset: Node = None) -> None:
        ...
    @typing.overload
    def __init__(self, frequency: Node, distribution: str, reset: Node = None) -> None:
        ...
class RandomImpulseSequence(StochasticNode):
    """
    Generates a random sequence of 0/1 bits with the given length, and the given probability each each bit = 1. The position of the sequence is incremented on each clock signal. explore and generate are trigger inputs which cause the sequence to mutate and re-generate respectively.
    """
    def __init__(self, probability: Node = 0.5, length: Node = 8, clock: Node = None, explore: Node = None, generate: Node = None, reset: Node = None) -> None:
        ...
class RandomUniform(StochasticNode):
    """
    Generates a uniformly random value between min/max. If a clock is passed, only picks a new value on a clock tick.
    """
    def __init__(self, min: Node = 0.0, max: Node = 1.0, clock: Node = None, reset: Node = None) -> None:
        ...
class RectangularEnvelope(Node):
    """
    Rectangular envelope with the given sustain duration.
    """
    def __init__(self, sustain_duration: Node = 1.0, clock: Node = None) -> None:
        ...
class Resample(Node):
    """
    Resampler and bit crusher. `sample_rate` is in Hz, `bit_rate` is an integer between 0 and 16.
    """
    def __init__(self, input: Node = 0, sample_rate: Node = 44100, bit_rate: Node = 16) -> None:
        ...
class Round(Node):
    """
    Round the input to the nearest integer value.
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class RoundToScale(Node):
    """
    Given a frequency input, generates a frequency output that is rounded to the nearest MIDI note. (TODO: Not very well named)
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class SVFilter(Node):
    """
    State variable filter. `filter_type` can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak', 'low_shelf', 'high_shelf'. `resonance` should be between `[0..1]`.
    """
    @typing.overload
    def __init__(self, input: Node = 0.0, filter_type: signalflow_filter_type_t = ..., cutoff: Node = 440, resonance: Node = 0.0) -> None:
        ...
    @typing.overload
    def __init__(self, input: Node, filter_type: str, cutoff: Node = 440, resonance: Node = 0.0) -> None:
        ...
class SampleAndHold(Node):
    """
    Samples and holds the input each time a trigger is received on `clock`.
    """
    def __init__(self, input: Node = None, clock: Node = None) -> None:
        ...
class SawLFO(Node):
    """
    Produces a sawtooth LFO at the given `frequency` and `phase` offset, with output ranging from `min` to `max`.
    """
    def __init__(self, frequency: Node = 1.0, min: Node = 0.0, max: Node = 1.0, phase: Node = 0.0) -> None:
        ...
class SawOscillator(Node):
    """
    Produces a (non-band-limited) sawtooth wave, with the given `frequency` and `phase` offset. When a `reset` or trigger is received, resets the phase to zero.
    """
    def __init__(self, frequency: Node = 440, phase: Node = None, reset: Node = None) -> None:
        ...
class ScaleLinExp(Node):
    """
    Scales the input from a linear range (between a and b) to an exponential range (between c and d).
    """
    def __init__(self, input: Node = 0, a: Node = 0, b: Node = 1, c: Node = 1, d: Node = 10) -> None:
        ...
class ScaleLinLin(Node):
    """
    Scales the input from a linear range (between a and b) to a linear range (between c and d).
    """
    def __init__(self, input: Node = 0, a: Node = 0, b: Node = 1, c: Node = 1, d: Node = 10) -> None:
        ...
class SegmentPlayer(Node):
    """
    Trigger segments of a buffer at the given onset positions.
    """
    def __init__(self, buffer: ... = None, onsets: list[float] = 0, index: Node = None, rate: Node = 1.0, clock: Node = None) -> None:
        ...
class SegmentedGranulator(Node):
    """
    Segmented Granulator.
    """
    def __init__(self, buffer: ... = None, onset_times: list[float] = 0, durations: list[float] = 0, index: Node = 0.0, rate: Node = 1.0, clock: Node = 0, max_grains: Node = 2048) -> None:
        ...
class Sequence(Node):
    """
    Outputs the elements in `sequence`, incrementing position on each `clock`.
    """
    def __init__(self: typing.Sequence, sequence: list[float] = [], clock: Node = None) -> None:
        ...
class Sin(Node):
    """
    Outputs sin(a), per sample.
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class SineLFO(Node):
    """
    Produces a sinusoidal LFO at the given `frequency` and `phase` offset, with output ranging from `min` to `max`.
    """
    def __init__(self, frequency: Node = 1.0, min: Node = 0.0, max: Node = 1.0, phase: Node = 0.0) -> None:
        ...
class SineOscillator(Node):
    """
    Produces a sine wave at the given `frequency`.
    """
    def __init__(self, frequency: Node = 440) -> None:
        ...
class Smooth(Node):
    """
    Smooth the input with a given smoothing coefficient. When `smooth` = 0, applies no smoothing.
    """
    def __init__(self, input: Node = None, smooth: Node = 0.99) -> None:
        ...
class SpatialEnvironment:
    def __init__(self) -> None:
        ...
    @typing.overload
    def add_speaker(self, arg0: int, arg1: float, arg2: float) -> None:
        ...
    @typing.overload
    def add_speaker(self, arg0: int, arg1: float, arg2: float, arg3: float) -> None:
        ...
class SpatialPanner(Node):
    """
    Implements a spatial panning algorithm, applied to a given SpatialEnvironment. Currently, only DBAP is supported.
    """
    def __init__(self, env: SpatialEnvironment = None, input: Node = 0.0, x: Node = 0.0, y: Node = 0.0, z: Node = 0.0, radius: Node = 1.0, algorithm: str = 'dbap') -> None:
        ...
class SpatialSpeaker:
    pass
class SquareLFO(Node):
    """
    Produces a pulse wave LFO with the given `frequency` and pulse `width`,  ranging from `min` to `max`, where `width` of `0.5` is a square wave and other values produce a rectangular wave.
    """
    def __init__(self, frequency: Node = 1.0, min: Node = 0.0, max: Node = 1.0, width: Node = 0.5, phase: Node = 0.0) -> None:
        ...
class SquareOscillator(Node):
    """
    Produces a pulse wave with the given `frequency` and pulse `width`,  where `width` of `0.5` is a square wave and other values produce a rectangular wave.
    """
    def __init__(self, frequency: Node = 440, width: Node = 0.5) -> None:
        ...
class Squiz(Node):
    """
    Implementation of Dan Stowell's Squiz algorithm, a kind of downsampler.
    """
    def __init__(self, input: Node = 0.0, rate: Node = 2.0, chunk_size: Node = 1) -> None:
        ...
class StereoBalance(Node):
    """
    Takes a stereo input and rebalances it, where `balance` of `0` is unchanged, `-1` is hard left, and `1` is hard right.
    """
    def __init__(self, input: Node = 0, balance: Node = 0) -> None:
        ...
class StereoPanner(Node):
    """
    Pans a mono input to a stereo output. `pan` should be between -1 (hard left) to +1 (hard right), with 0 = centre.
    """
    def __init__(self, input: Node = 0, pan: Node = 0.0) -> None:
        ...
class StereoWidth(Node):
    """
    Reduces the width of a stereo signal. When `width` = 1, input is unchanged. When `width` = 0, outputs a pair of identical channels both containing L+R.
    """
    def __init__(self, input: Node = 0, width: Node = 1) -> None:
        ...
class StochasticNode(Node):
    def set_seed(self, arg0: int) -> None:
        ...
class Stutter(Node):
    """
    Stutters the input whenever a trigger is received on `clock`. Generates `stutter_count` repeats, with duration of `stutter_time`.
    """
    def __init__(self, input: Node = 0.0, stutter_time: Node = 0.1, stutter_count: Node = 1, clock: Node = None, max_stutter_time: float = 1.0) -> None:
        ...
class Subtract(Node):
    """
    Subtract each sample of b from each sample of a. Can also be written as a - b
    """
    def __init__(self, a: Node = 0, b: Node = 0) -> None:
        ...
class Sum(Node):
    """
    Sums the output of all of the input nodes, by sample.
    """
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, inputs: ...) -> None:
        ...
    @typing.overload
    def __init__(self, inputs: list[Node]) -> None:
        ...
    @typing.overload
    def __init__(self, inputs: list[int]) -> None:
        ...
    @typing.overload
    def __init__(self, inputs: list[float]) -> None:
        ...
class Tan(Node):
    """
    Outputs tan(a), per sample.
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class Tanh(Node):
    """
    Outputs tanh(a), per sample. Can be used as a soft clipper.
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class TimeShift(Node):
    """
    TimeShift
    """
    def __init__(self, a: Node = 0) -> None:
        ...
class TriangleLFO(Node):
    """
    Produces a triangle LFO with the given `frequency` and `phase` offset, ranging from `min` to `max`.
    """
    def __init__(self, frequency: Node = 1.0, min: Node = 0.0, max: Node = 1.0, phase: Node = 0.0) -> None:
        ...
class TriangleOscillator(Node):
    """
    Produces a triangle wave with the given `frequency`.
    """
    def __init__(self, frequency: Node = 440) -> None:
        ...
class VampAnalysis(Node):
    """
    Feature extraction using the Vamp plugin toolkit.
    """
    def __init__(self, input: Node = 0.0, plugin_id: str = 'vamp-example-plugins:spectralcentroid:linearcentroid') -> None:
        ...
    def list_plugins(self) -> list[str]:
        """
        list[str]: List the available plugin names.
        """
class WaveShaper(Node):
    """
    Applies wave-shaping as described in the WaveShaperBuffer `buffer`.
    """
    def __init__(self, input: Node = 0.0, buffer: ... = None) -> None:
        ...
class WaveShaperBuffer(Buffer):
    """
    Sample buffer for waveshaper nodes
    """
    @typing.overload
    def __init__(self, num_frames: int) -> None:
        """
        Create a waveshaper buffer containing the given number of samples.
        """
    @typing.overload
    def __init__(self, function: typing.Callable[[float], float]) -> None:
        """
        Create a waveshaper buffer filled with the output of a given function.
        """
    def get(self, arg0: float) -> float:
        ...
    @property
    def frame_offsets(self) -> list[float]:
        """
        Returns a list containing the offset in the waveshaper buffer for each frame, ranging over -1..1.
        """
class Wavetable(Node):
    """
    Plays the wavetable stored in buffer at the given `frequency` and `phase` offset. `sync` can be used to provide a hard sync input, which resets the wavetable's phase at each zero-crossing.
    """
    def __init__(self, buffer: ... = None, frequency: Node = 440, phase: Node = 0, sync: Node = 0, phase_map: ... = None) -> None:
        ...
class Wavetable2D(Node):
    """
    Wavetable2D
    """
    def __init__(self, buffer: ... = None, frequency: Node = 440, crossfade: Node = 0.0, phase: Node = 0.0, sync: Node = 0) -> None:
        ...
class WetDry(Node):
    """
    Takes `wet` and `dry` inputs, and outputs a mix determined by `wetness`.
    """
    def __init__(self, dry_input: Node = None, wet_input: Node = None, wetness: Node = 0.0) -> None:
        ...
class WhiteNoise(StochasticNode):
    """
    Generates whitenoise between min/max. If frequency is zero, generates at audio rate. For frequencies lower than audio rate, interpolate applies linear interpolation between values, and random_interval specifies whether new random values should be equally-spaced or randomly-spaced.
    """
    def __init__(self, frequency: Node = 0.0, min: Node = -1.0, max: Node = 1.0, interpolate: bool = True, random_interval: bool = True, reset: Node = None) -> None:
        ...
class Wrap(Node):
    """
    Wrap the input beyond `min`/`max`.
    """
    def __init__(self, input: Node = None, min: Node = -1.0, max: Node = 1.0) -> None:
        ...
class signalflow_event_distribution_t:
    """
    signalflow_event_distribution_t
    
    Members:
    
      SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM : Uniform distribution
    
      SIGNALFLOW_EVENT_DISTRIBUTION_POISSON : Poisson distribution
    """
    SIGNALFLOW_EVENT_DISTRIBUTION_POISSON: typing.ClassVar[signalflow_event_distribution_t]  # value = <signalflow_event_distribution_t.SIGNALFLOW_EVENT_DISTRIBUTION_POISSON: 1>
    SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM: typing.ClassVar[signalflow_event_distribution_t]  # value = <signalflow_event_distribution_t.SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM: 0>
    __members__: typing.ClassVar[dict[str, signalflow_event_distribution_t]]  # value = {'SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM': <signalflow_event_distribution_t.SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM: 0>, 'SIGNALFLOW_EVENT_DISTRIBUTION_POISSON': <signalflow_event_distribution_t.SIGNALFLOW_EVENT_DISTRIBUTION_POISSON: 1>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class signalflow_filter_type_t:
    """
    Filter type
    
    Members:
    
      SIGNALFLOW_FILTER_TYPE_LOW_PASS : Low-pass filter
    
      SIGNALFLOW_FILTER_TYPE_HIGH_PASS : High-pass filter
    
      SIGNALFLOW_FILTER_TYPE_BAND_PASS : Band-pass filter
    
      SIGNALFLOW_FILTER_TYPE_NOTCH : Notch filter
    
      SIGNALFLOW_FILTER_TYPE_PEAK : Peak filter
    
      SIGNALFLOW_FILTER_TYPE_LOW_SHELF : Low-shelf filter
    
      SIGNALFLOW_FILTER_TYPE_HIGH_SHELF : High-shelf filter
    """
    SIGNALFLOW_FILTER_TYPE_BAND_PASS: typing.ClassVar[signalflow_filter_type_t]  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_BAND_PASS: 2>
    SIGNALFLOW_FILTER_TYPE_HIGH_PASS: typing.ClassVar[signalflow_filter_type_t]  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_HIGH_PASS: 1>
    SIGNALFLOW_FILTER_TYPE_HIGH_SHELF: typing.ClassVar[signalflow_filter_type_t]  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_HIGH_SHELF: 6>
    SIGNALFLOW_FILTER_TYPE_LOW_PASS: typing.ClassVar[signalflow_filter_type_t]  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_LOW_PASS: 0>
    SIGNALFLOW_FILTER_TYPE_LOW_SHELF: typing.ClassVar[signalflow_filter_type_t]  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_LOW_SHELF: 5>
    SIGNALFLOW_FILTER_TYPE_NOTCH: typing.ClassVar[signalflow_filter_type_t]  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_NOTCH: 3>
    SIGNALFLOW_FILTER_TYPE_PEAK: typing.ClassVar[signalflow_filter_type_t]  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_PEAK: 4>
    __members__: typing.ClassVar[dict[str, signalflow_filter_type_t]]  # value = {'SIGNALFLOW_FILTER_TYPE_LOW_PASS': <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_LOW_PASS: 0>, 'SIGNALFLOW_FILTER_TYPE_HIGH_PASS': <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_HIGH_PASS: 1>, 'SIGNALFLOW_FILTER_TYPE_BAND_PASS': <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_BAND_PASS: 2>, 'SIGNALFLOW_FILTER_TYPE_NOTCH': <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_NOTCH: 3>, 'SIGNALFLOW_FILTER_TYPE_PEAK': <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_PEAK: 4>, 'SIGNALFLOW_FILTER_TYPE_LOW_SHELF': <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_LOW_SHELF: 5>, 'SIGNALFLOW_FILTER_TYPE_HIGH_SHELF': <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_HIGH_SHELF: 6>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class signalflow_interpolation_mode_t:
    """
    signalflow_interpolation_mode_t
    
    Members:
    
      SIGNALFLOW_INTERPOLATION_MODE_NONE : No interpolation
    
      SIGNALFLOW_INTERPOLATION_MODE_LINEAR : Linear interpolation
    
      SIGNALFLOW_INTERPOLATION_MODE_COSINE : Cosine interpolation
    """
    SIGNALFLOW_INTERPOLATION_MODE_COSINE: typing.ClassVar[signalflow_interpolation_mode_t]  # value = <signalflow_interpolation_mode_t.SIGNALFLOW_INTERPOLATION_MODE_COSINE: 2>
    SIGNALFLOW_INTERPOLATION_MODE_LINEAR: typing.ClassVar[signalflow_interpolation_mode_t]  # value = <signalflow_interpolation_mode_t.SIGNALFLOW_INTERPOLATION_MODE_LINEAR: 1>
    SIGNALFLOW_INTERPOLATION_MODE_NONE: typing.ClassVar[signalflow_interpolation_mode_t]  # value = <signalflow_interpolation_mode_t.SIGNALFLOW_INTERPOLATION_MODE_NONE: 0>
    __members__: typing.ClassVar[dict[str, signalflow_interpolation_mode_t]]  # value = {'SIGNALFLOW_INTERPOLATION_MODE_NONE': <signalflow_interpolation_mode_t.SIGNALFLOW_INTERPOLATION_MODE_NONE: 0>, 'SIGNALFLOW_INTERPOLATION_MODE_LINEAR': <signalflow_interpolation_mode_t.SIGNALFLOW_INTERPOLATION_MODE_LINEAR: 1>, 'SIGNALFLOW_INTERPOLATION_MODE_COSINE': <signalflow_interpolation_mode_t.SIGNALFLOW_INTERPOLATION_MODE_COSINE: 2>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class signalflow_node_state_t:
    """
    signalflow_node_state_t
    
    Members:
    
      SIGNALFLOW_NODE_STATE_ACTIVE : Active
    
      SIGNALFLOW_NODE_STATE_STOPPED : Stopped
    """
    SIGNALFLOW_NODE_STATE_ACTIVE: typing.ClassVar[signalflow_node_state_t]  # value = <signalflow_node_state_t.SIGNALFLOW_NODE_STATE_ACTIVE: 0>
    SIGNALFLOW_NODE_STATE_STOPPED: typing.ClassVar[signalflow_node_state_t]  # value = <signalflow_node_state_t.SIGNALFLOW_NODE_STATE_STOPPED: 1>
    __members__: typing.ClassVar[dict[str, signalflow_node_state_t]]  # value = {'SIGNALFLOW_NODE_STATE_ACTIVE': <signalflow_node_state_t.SIGNALFLOW_NODE_STATE_ACTIVE: 0>, 'SIGNALFLOW_NODE_STATE_STOPPED': <signalflow_node_state_t.SIGNALFLOW_NODE_STATE_STOPPED: 1>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class signalflow_patch_state_t:
    """
    signalflow_patch_state_t
    
    Members:
    
      SIGNALFLOW_PATCH_STATE_ACTIVE : Active
    
      SIGNALFLOW_PATCH_STATE_STOPPED : Stopped
    """
    SIGNALFLOW_PATCH_STATE_ACTIVE: typing.ClassVar[signalflow_patch_state_t]  # value = <signalflow_patch_state_t.SIGNALFLOW_PATCH_STATE_ACTIVE: 0>
    SIGNALFLOW_PATCH_STATE_STOPPED: typing.ClassVar[signalflow_patch_state_t]  # value = <signalflow_patch_state_t.SIGNALFLOW_PATCH_STATE_STOPPED: 1>
    __members__: typing.ClassVar[dict[str, signalflow_patch_state_t]]  # value = {'SIGNALFLOW_PATCH_STATE_ACTIVE': <signalflow_patch_state_t.SIGNALFLOW_PATCH_STATE_ACTIVE: 0>, 'SIGNALFLOW_PATCH_STATE_STOPPED': <signalflow_patch_state_t.SIGNALFLOW_PATCH_STATE_STOPPED: 1>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
def amplitude_to_db(arg0: float) -> float:
    """
    Map linear amplitude to decibels
    """
def clip(arg0: float, arg1: float, arg2: float) -> float:
    """
    Clip arg0 between [arg1..arg2]
    """
def db_to_amplitude(arg0: float) -> float:
    """
    Map decibels to linear amplitude
    """
def fold(arg0: float, arg1: float, arg2: float) -> float:
    """
    Fold arg0 between [arg1..arg2]
    """
def frequency_to_midi_note(arg0: float) -> float:
    """
    Map frequency in Hz to a MIDI note value
    """
def midi_note_to_frequency(arg0: float) -> float:
    """
    Map MIDI note value to a frequency in Hz
    """
def random_exponential(arg0: float, arg1: float) -> float:
    """
    Return a random number, exponentially distributed across a fixed range
    """
def random_seed(arg0: int) -> None:
    """
    Set the global random seed
    """
@typing.overload
def random_uniform() -> float:
    """
    Return a uniformly random number
    """
@typing.overload
def random_uniform(arg0: float, arg1: float) -> float:
    """
    Return a uniformly random number
    """
def save_block_to_text_file(arg0: float, arg1: int, arg2: str) -> None:
    """
    Write a block of PCM float samples to a .csv-style text file
    """
def save_block_to_wav_file(arg0: float, arg1: int, arg2: str) -> None:
    """
    Write a block of PCM float samples to a .wav file
    """
def scale_exp_lin(arg0: float, arg1: float, arg2: float, arg3: float, arg4: float) -> float:
    """
    Scale arg0, from exponential range [arg1..arg2] to linear range [arg3..arg4]
    """
def scale_lin_exp(arg0: float, arg1: float, arg2: float, arg3: float, arg4: float) -> float:
    """
    Scale arg0, from linear range [arg1..arg2] to exponential range [arg3..arg4]
    """
def scale_lin_lin(arg0: float, arg1: float, arg2: float, arg3: float, arg4: float) -> float:
    """
    Scale arg0, from linear range [arg1..arg2] to linear range [arg3..arg4]
    """
def wrap(arg0: float, arg1: float, arg2: float) -> float:
    """
    Wrap arg0 between [arg1..arg2]
    """
SIGNALFLOW_DEFAULT_BLOCK_SIZE: int = 256
SIGNALFLOW_DEFAULT_FFT_HOP_SIZE: int = 128
SIGNALFLOW_DEFAULT_FFT_SIZE: int = 1024
SIGNALFLOW_DEFAULT_SAMPLE_RATE: int = 44100
SIGNALFLOW_DEFAULT_TRIGGER: str = 'trigger'
SIGNALFLOW_EVENT_DISTRIBUTION_POISSON: signalflow_event_distribution_t  # value = <signalflow_event_distribution_t.SIGNALFLOW_EVENT_DISTRIBUTION_POISSON: 1>
SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM: signalflow_event_distribution_t  # value = <signalflow_event_distribution_t.SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM: 0>
SIGNALFLOW_FILTER_TYPE_BAND_PASS: signalflow_filter_type_t  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_BAND_PASS: 2>
SIGNALFLOW_FILTER_TYPE_HIGH_PASS: signalflow_filter_type_t  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_HIGH_PASS: 1>
SIGNALFLOW_FILTER_TYPE_HIGH_SHELF: signalflow_filter_type_t  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_HIGH_SHELF: 6>
SIGNALFLOW_FILTER_TYPE_LOW_PASS: signalflow_filter_type_t  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_LOW_PASS: 0>
SIGNALFLOW_FILTER_TYPE_LOW_SHELF: signalflow_filter_type_t  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_LOW_SHELF: 5>
SIGNALFLOW_FILTER_TYPE_NOTCH: signalflow_filter_type_t  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_NOTCH: 3>
SIGNALFLOW_FILTER_TYPE_PEAK: signalflow_filter_type_t  # value = <signalflow_filter_type_t.SIGNALFLOW_FILTER_TYPE_PEAK: 4>
SIGNALFLOW_INTERPOLATION_MODE_COSINE: signalflow_interpolation_mode_t  # value = <signalflow_interpolation_mode_t.SIGNALFLOW_INTERPOLATION_MODE_COSINE: 2>
SIGNALFLOW_INTERPOLATION_MODE_LINEAR: signalflow_interpolation_mode_t  # value = <signalflow_interpolation_mode_t.SIGNALFLOW_INTERPOLATION_MODE_LINEAR: 1>
SIGNALFLOW_INTERPOLATION_MODE_NONE: signalflow_interpolation_mode_t  # value = <signalflow_interpolation_mode_t.SIGNALFLOW_INTERPOLATION_MODE_NONE: 0>
SIGNALFLOW_MAX_CHANNELS: int = 64
SIGNALFLOW_MAX_FFT_SIZE: int = 131072
SIGNALFLOW_NODE_BUFFER_SIZE: int = 2048
SIGNALFLOW_NODE_STATE_ACTIVE: signalflow_node_state_t  # value = <signalflow_node_state_t.SIGNALFLOW_NODE_STATE_ACTIVE: 0>
SIGNALFLOW_NODE_STATE_STOPPED: signalflow_node_state_t  # value = <signalflow_node_state_t.SIGNALFLOW_NODE_STATE_STOPPED: 1>
SIGNALFLOW_PATCH_STATE_ACTIVE: signalflow_patch_state_t  # value = <signalflow_patch_state_t.SIGNALFLOW_PATCH_STATE_ACTIVE: 0>
SIGNALFLOW_PATCH_STATE_STOPPED: signalflow_patch_state_t  # value = <signalflow_patch_state_t.SIGNALFLOW_PATCH_STATE_STOPPED: 1>
__version__: str = '0.4.5'
