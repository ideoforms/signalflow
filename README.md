# SignalFlow

![ci](https://github.com/ideoforms/signal/workflows/ci/badge.svg)

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

### macOS

To install the Python library on macOS, install dependencies with homebrew:
```
brew install cmake python
```

Clone this repository, then build and install with `pip`:
```
pip3 install .
```


### Linux, Raspberry Pi

To install the Python library on Linux, install dependencies with apt:
```
apt-get install git cmake g++ python3-pip libasound2-dev libsndfile1-dev libsoundio-dev fftw3-dev
```

If you experience an error on Raspberry Pi `libf77blas.so.3: cannot open shared object file`:

```
sudo apt-get install libatlas-base-dev
```

Clone this repository, then build and install with `pip`:
```
pip3 install .
```

#### Windows

This is work in progress.

Currently, dependencies need to be downloaded and built by hand. These can be placed anywhere.

- https://github.com/timmb/libsoundio - check out the `fix-msvc` branch.
  - Use CMake GUI to build libsoundio with Visual Studio 2019 with binaries in a subfolder of that repo named `build`. (Configure, Generate, Open project, Batch build all configurations)
- https://github.com/libsndfile/libsndfile
  - Use CMake GUI to build libsndfile with Visual Studio 2019 with binaries in a subfolder of that repo named `build`. (Configure, Generate, Open project, Batch build all configurations)
- Download Windows binaries of FFTW from http://fftw.org/install/windows.html.

To build SignalFlow, use the CMake GUI. Press configure and you will see three empty fields to fill in with the path to the two build folders and the FFTW binaries folder (see above). Set these parameters then press Configure, then Generate then Open. Then build in Visual Studio 2019.

As of 2021-03-03, only the signalflow project has been ported to build correctly on Windows. Only tested in x64 and for Debug builds. Tested using Visual Studio 2019.

## Build (C++)

To build and install the C++ core without the Python binding layer:
```
mkdir build
cd build
cmake ..
make -j8
```

## Examples

See [examples](examples) for a number of example programs.

To run an example:
```
cd build
./hello-world
```

## Documentation

Documentation is in the works.

## Node classes

The following Node classes are currently available:

```
Abs
Add
AllpassDelay
AmplitudeToDecibels
AudioIn
AudioOut_Abstract
    AudioOut
    AudioOut_Dummy
BiquadFilter
BufferPlayer
BufferRecorder
ChannelArray
ChannelMixer
ChannelSelect
Clip
ClockDivider
CombDelay
Compressor
Constant
Counter
CrossCorrelate
DecibelsToAmplitude
Divide
EQ
Envelope
ADSREnvelope
ASREnvelope
Equal
Euclidean
FFT
FFTContinuousPhaseVocoder
FFTConvolve
FFTFindPeaks
FFTLPF
FFTPhaseVocoder
FFTTonality
FlipFlop
Fold
Gate
Granulator
GreaterThan
GreaterThanOrEqual
IFFT
If
Impulse
ImpulseSequence
Index
LFO
Latch
LessThan
LessThanOrEqual
Line
LinearPanner
Logistic
Maximiser
MidiNoteToFrequency
Modulo
MoogVCF
MouseDown
MouseX
MouseY
Multiply
NotEqual
OneTapDelay
OnsetDetector
Pow
RMS
Resample
RoundToScale
SVFilter
SampleAndHold
Saw
SawLFO
ScaleLinExp
ScaleLinLin
Sine
SineLFO
Smooth
Square
SquareLFO
Squiz
StereoBalance
StereoWidth
StochasticNode
    PinkNoise
    RandomBrownian
    RandomCoin
    RandomExponential
    RandomExponentialDist
    RandomGaussian
    RandomImpulse
    RandomImpulseSequence
    RandomUniform
    WhiteNoise
Stutter
Subtract
Sum
Tanh
Triangle
TriangleLFO
WaveShaper
Wavetable
Wavetable2D
WetDry
Wrap
```

