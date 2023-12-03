# SignalFlow

![ci](https://github.com/ideoforms/signal/workflows/ci/badge.svg) [![stability-beta](https://img.shields.io/badge/stability-beta-33bbff.svg)](https://github.com/mkenney/software-guides/blob/master/STABILITY-BADGES.md#beta)

SignalFlow is a sound synthesis framework designed for clear and concise expression of complex musical ideas. It has an extensive Python API, for fluid audio experimentation in iPython/Jupyter. Its core is written in C++11, so it can also be embedded into other cross-platform applications.

SignalFlow is in beta status. Interfaces may be subject to change.

## Example

```python
from signalflow import *

#--------------------------------------------------------------------------------
# An AudioGraph is made up of a network of interconnected Nodes, which generate
# and process audio. 
#--------------------------------------------------------------------------------
graph = AudioGraph()

#--------------------------------------------------------------------------------
# Passing an array of frequencies creates a stereo output.
#--------------------------------------------------------------------------------
sine = SineOscillator([440, 880])

#--------------------------------------------------------------------------------
# Simple attack/sustain/release envelope with linear curves.
#--------------------------------------------------------------------------------
env = ASREnvelope(0.01, 0.1, 0.5)

#--------------------------------------------------------------------------------
# Use standard arithmetic operations to combine signals. When a multi-channel 
# signal is multiplied by a mono signal, the mono signal is auto-upmixed.
#--------------------------------------------------------------------------------
output = sine * env

#--------------------------------------------------------------------------------
# Connect the output to the graph, and begin playback.
#--------------------------------------------------------------------------------
output.play()
graph.wait()
```

## Installation

SignalFlow requires Python 3.8 or above.

In brief, SignalFlow can be installed with pip: `pip3 install signalflow`.

For complete per-platform instructions and setup guides, see [Installing SignalFlow](https://signalflow.dev/installation/).

## Examples

Several example scripts are provided in [examples](examples).

To download the `examples` folder:

```python
import signalflow_examples
signalflow_examples.download_examples()
```

## Documentation

In-depth documentation, including a reference guide to each of the Node classes, can be found at [signalflow.dev](https://signalflow.dev/). 

## Node classes

The following Node classes are currently included with the base distribution:

| Category | Classes  |
|:---------|:---------|
| **Analysis** | CrossCorrelate, OnsetDetector, VampAnalysis |
| **Buffer** | BeatCutter, BufferLooper, BufferPlayer, BufferRecorder, FeedbackBufferReader, FeedbackBufferWriter, GrainSegments, Granulator, SegmentPlayer |
| **Control** | MouseX, MouseY, MouseDown |
| **Envelope** | ADSREnvelope, ASREnvelope, DetectSilence, Envelope, Line, RectangularEnvelope |
| **FFT** | FFTContinuousPhaseVocoder, FFTConvolve, FFT, FFTNode, FFTOpNode, FFTFindPeaks, IFFT, FFTLPF, FFTNoiseGate, FFTPhaseVocoder, FFTTonality, FFTZeroPhase |
| **Operators** | Add, AmplitudeToDecibels, DecibelsToAmplitude, ChannelArray, ChannelCrossfade, ChannelMixer, ChannelSelect, Equal, NotEqual, GreaterThan, GreaterThanOrEqual, LessThan, LessThanOrEqual, Modulo, Abs, If, Divide, FrequencyToMidiNote, MidiNoteToFrequency, Multiply, Pow, RoundToScale, Round, ScaleLinExp, ScaleLinLin, Subtract, Sum, Sin, Cos, Tan, Tanh |
| **Oscillators** | Constant, Impulse, LFO, SawLFO, SawOscillator, SineLFO, SineOscillator, SquareLFO, SquareOscillator, TriangleLFO, TriangleOscillator, Wavetable, Wavetable2D |
| **Processors** | Clip, Fold, Smooth, WetDry, Wrap |
| **Processors: Delays** | AllpassDelay, CombDelay, OneTapDelay, Stutter |
| **Processors: Distortion** | Resample, SampleAndHold, Squiz, WaveShaper |
| **Processors: Dynamics** | Compressor, Gate, Maximiser, RMS |
| **Processors: Filters** | BiquadFilter, DCFilter, EQ, MoogVCF, SVFilter |
| **Processors: Panning** | AzimuthPanner, ChannelPanner, SpatialPanner, StereoBalance, StereoPanner, StereoWidth |
| **Sequencing** | ClockDivider, Counter, Euclidean, FlipFlop, ImpulseSequence, Index, Latch, Sequence |
| **Stochastic** | Logistic, PinkNoise, RandomBrownian, RandomChoice, RandomCoin, RandomExponentialDist, RandomExponential, RandomGaussian, RandomImpulseSequence, RandomImpulse, RandomUniform, StochasticNode, WhiteNoise |

## Contributors

Thanks to the following contributors:

- Arthur Carabott ([@acarabott](https://github.com/acarabott)), Tim Murray-Browne ([@timmb](https://github.com/timmb)): Windows support
- Dan Stowell ([@danstowell](https://github.com/danstowell)): Build improvements
- Marc Fargas ([@telenieko](https://github.com/telenieko)): Generous donation of the `signalflow` pypi namespace
