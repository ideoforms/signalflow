# SignalFlow

![ci](https://github.com/ideoforms/signal/workflows/ci/badge.svg) [![stability-beta](https://img.shields.io/badge/stability-beta-33bbff.svg)](https://github.com/mkenney/software-guides/blob/master/STABILITY-BADGES.md#beta)

SignalFlow is a sound synthesis framework designed for clear and concise expression of complex musical ideas. It has an extensive Python API, for fluid audio experimentation in iPython/Jupyter. Its core is written in C++11, so it can also be embedded into other cross-platform applications.

SignalFlow is in alpha status. All interfaces may be subject to change.

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

<details>
<summary>Installation: <b>macOS</b></summary>

### macOS

Install an up-to-date version of Python 3 using Homebrew ([guide](https://docs.python-guide.org/starting/install3/osx/)): `brew install python3`

Install SignalFlow using `pip`:

```
pip3 install signalflow
```

That's it! To test that it is installed successfully, play a test tone by running: `signalflow test` 

#### From source

To build on macOS from source, install dependencies with Homebrew: 
```
brew install cmake python libsndfile libsoundio
```

Clone this repository, then build and install with `pip`:
```
pip3 install .
```
</details>

<details>
<summary>Installation: <b>Linux, Raspberry Pi</b></summary>

### Linux, Raspberry Pi

SignalFlow supports Linux (verified on Ubuntu 20.04 and Raspberry Pi OS buster) with alsa and pulseaudio backends.

#### Via pip

Installation via pip is supported for Linux x86_64. To install:

```
pip3 install signalflow
```

That's it! To test that it is installed successfully, play a test tone by running: `signalflow test`.

If installation does not succeed, follow the instructions for building from source below. 

#### From source

To build the Python library from source on Linux, install dependencies with apt:
```
apt-get install -y git cmake g++ python3-pip libasound2-dev libsndfile1-dev libsoundio-dev fftw3-dev
```

If you experience an error on Raspberry Pi `libf77blas.so.3: cannot open shared object file`:

```
sudo apt-get install -y libatlas-base-dev
```

Clone this repository, then build and install with `pip`:
```
pip3 install .
```

</details>

<details>
<summary>Installation: <b>Windows</b></summary>

### Windows

This is work in progress.

Currently, dependencies need to be downloaded and built by hand. These can be placed anywhere.

- https://github.com/timmb/libsoundio - check out the `fix-msvc` branch.
  - Use CMake GUI to build libsoundio with Visual Studio 2019 with binaries in a subfolder of that repo named `build`. (Configure, Generate, Open project, Batch build all configurations)
- https://github.com/libsndfile/libsndfile
  - Use CMake GUI to build libsndfile with Visual Studio 2019 with binaries in a subfolder of that repo named `build`. (Configure, Generate, Open project, Batch build all configurations)
- Download Windows binaries of FFTW from http://fftw.org/install/windows.html.

To build SignalFlow, use the CMake GUI. Press configure and you will see three empty fields to fill in with the path to the two build folders and the FFTW binaries folder (see above). Set these parameters then press Configure, then Generate then Open. Then build in Visual Studio 2019.

As of 2021-03-03, only the signalflow project has been ported to build correctly on Windows. Only tested in x64 and for Debug builds. Tested using Visual Studio 2019.

</details>

<details>
<summary>Installation: <b>C++ only</b> (no Python layer)</summary>

To build and install the C++ core without the Python binding layer:
```
mkdir build
cd build
cmake ..
make -j8
```

</details>

## Examples

See [examples/python](examples/python) for a number of example scripts.

## Documentation

Work-in-progress documentation can be found at [ideoforms.github.io/signalflow/](https://ideoforms.github.io/signalflow/). 

## Node classes

The following Node classes are currently included with the base distribution:

| Category | Classes  |
|:---------|:---------|
| **Analysis** | CrossCorrelate, OnsetDetector, VampAnalysis |
| **Buffer** | BeatCutter, BufferLooper, BufferPlayer, BufferRecorder, FeedbackBufferReader, FeedbackBufferWriter, GrainSegments, Granulator, SegmentPlayer |
| **Control** | MouseX, MouseY, MouseDown |
| **Envelope** | ADSREnvelope, ASREnvelope, DetectSilence, Envelope, Line, EnvelopeRect |
| **Fft** | FFTContinuousPhaseVocoder, FFTConvolve, FFT, FFTNode, FFTOpNode, FFTFindPeaks, IFFT, FFTLPF, FFTNoiseGate, FFTPhaseVocoder, FFTTonality, FFTZeroPhase |
| **Operators** | Add, AmplitudeToDecibels, DecibelsToAmplitude, ChannelArray, ChannelMixer, ChannelSelect, Equal, NotEqual, GreaterThan, GreaterThanOrEqual, LessThan, LessThanOrEqual, Modulo, Abs, If, Divide, FrequencyToMidiNote, MidiNoteToFrequency, Multiply, Pow, RoundToScale, Round, ScaleLinExp, ScaleLinLin, Subtract, Sum, Sin, Cos, Tan, Tanh |
| **Oscillators** | Constant, Impulse, LFO, SawLFO, SawOscillator, SineLFO, SineOscillator, SquareLFO, SquareOscillator, TriangleLFO, TriangleOscillator, Wavetable, Wavetable2D |
| **Processors** | Clip, Fold, Smooth, WetDry, Wrap |
| **Processors: Delays** | AllpassDelay, CombDelay, OneTapDelay, Stutter |
| **Processors: Distortion** | Resample, SampleAndHold, Squiz, WaveShaper |
| **Processors: Dynamics** | Compressor, Gate, Maximiser, RMS |
| **Processors: Filters** | BiquadFilter, EQ, MoogVCF, SVFilter |
| **Processors: Panning** | AzimuthPanner, ChannelPanner, SpatialPanner, StereoBalance, StereoPanner, StereoWidth |
| **Sequencing** | ClockDivider, Counter, Euclidean, FlipFlop, ImpulseSequence, Index, Latch, Sequence |
| **Stochastic** | Logistic, PinkNoise, RandomBrownian, RandomChoice, RandomCoin, RandomExponentialDist, RandomExponential, RandomGaussian, RandomImpulseSequence, RandomImpulse, RandomUniform, StochasticNode, WhiteNoise |

## Contributors

Thanks to the following contributors:

- Arthur Carabott ([@acarabott](https://github.com/acarabott)), Tim Murray-Browne ([@timmb](https://github.com/timmb)): Windows support
- Dan Stowell ([@danstowell](https://github.com/danstowell)): Build improvements
- Marc Fargas ([@telenieko](https://github.com/telenieko)): Generous donation of the `signalflow` pypi namespace
