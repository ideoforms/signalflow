# SignalFlow

![pypi-version](https://img.shields.io/pypi/v/signalflow) ![ci](https://github.com/ideoforms/signalflow/actions/workflows/build.yml/badge.svg) [![stability-beta](https://img.shields.io/badge/stability-beta-33bbff.svg)](https://github.com/mkenney/software-guides/blob/master/STABILITY-BADGES.md#beta)

SignalFlow is a sound synthesis framework designed for clear and concise expression of complex musical ideas. It has an extensive Python API, for fluid audio experimentation in iPython/Jupyter, with its core written in portable, hardware-accelerated C++11.

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

SignalFlow requires Python 3.8 or above, and supports macOS, Linux (x86_64), Raspberry Pi, and Windows (64-bit).

In a nutshell, SignalFlow can be installed with pip: `pip3 install signalflow`.

For a complete walkthrough on setting up your system with Python and SignalFlow, see [Installing SignalFlow](https://signalflow.dev/installation/).

## Examples

Several example scripts are provided in [examples](examples).

To download the `examples` folder:

```python
import signalflow_examples
signalflow_examples.download_examples()
```

## Documentation

In-depth documentation, including a reference guide to each of the Node classes, can be found at [signalflow.dev](https://signalflow.dev/). 

## Node class library

The following Node classes are currently included with the base distribution:

| Category | Classes  |
|:---------|:---------|
| **Analysis** | [CrossCorrelate](https://signalflow.dev/library/analysis/crosscorrelate/), [NearestNeighbour](https://signalflow.dev/library/analysis/nearestneighbour/), [OnsetDetector](https://signalflow.dev/library/analysis/onsetdetector/), [VampAnalysis](https://signalflow.dev/library/analysis/vampanalysis/) |
| **Buffer** | [BeatCutter](https://signalflow.dev/library/buffer/beatcutter/), [BufferLooper](https://signalflow.dev/library/buffer/bufferlooper/), [BufferPlayer](https://signalflow.dev/library/buffer/bufferplayer/), [BufferRecorder](https://signalflow.dev/library/buffer/bufferrecorder/), [FeedbackBufferReader](https://signalflow.dev/library/buffer/feedbackbufferreader/), [FeedbackBufferWriter](https://signalflow.dev/library/buffer/feedbackbufferwriter/), [HistoryBufferWriter](https://signalflow.dev/library/buffer/historybufferwriter/), [SegmentPlayer](https://signalflow.dev/library/buffer/segmentplayer/) |
| **Buffer: Granulation** | [SegmentedGranulator](https://signalflow.dev/library/buffer/granulation/segmentedgranulator/), [Granulator](https://signalflow.dev/library/buffer/granulation/granulator/) |
| **Control** | [MouseX](https://signalflow.dev/library/control/mousex/), [MouseY](https://signalflow.dev/library/control/mousey/), [MouseDown](https://signalflow.dev/library/control/mousedown/) |
| **Envelope** | [Accumulator](https://signalflow.dev/library/envelope/accumulator/), [ADSREnvelope](https://signalflow.dev/library/envelope/adsrenvelope/), [ASREnvelope](https://signalflow.dev/library/envelope/asrenvelope/), [DetectSilence](https://signalflow.dev/library/envelope/detectsilence/), [Envelope](https://signalflow.dev/library/envelope/envelope/), [Line](https://signalflow.dev/library/envelope/line/), [RectangularEnvelope](https://signalflow.dev/library/envelope/rectangularenvelope/) |
| **FFT** | [FFTContinuousPhaseVocoder](https://signalflow.dev/library/fft/fftcontinuousphasevocoder/), [FFTConvolve](https://signalflow.dev/library/fft/fftconvolve/), [FFTBufferPlayer](https://signalflow.dev/library/fft/fftbufferplayer/), [FFTContrast](https://signalflow.dev/library/fft/fftcontrast/), [FFTCrossFade](https://signalflow.dev/library/fft/fftcrossfade/), [FFTLFO](https://signalflow.dev/library/fft/fftlfo/), [FFTMagnitudePhaseArray](https://signalflow.dev/library/fft/fftmagnitudephasearray/), [FFTRandomPhase](https://signalflow.dev/library/fft/fftrandomphase/), [FFTScaleMagnitudes](https://signalflow.dev/library/fft/fftscalemagnitudes/), [FFTTransform](https://signalflow.dev/library/fft/ffttransform/), [FFT](https://signalflow.dev/library/fft/fft/), [FFTNode](https://signalflow.dev/library/fft/fftnode/), [FFTOpNode](https://signalflow.dev/library/fft/fftopnode/), [FFTFindPeaks](https://signalflow.dev/library/fft/fftfindpeaks/), [IFFT](https://signalflow.dev/library/fft/ifft/), [FFTLPF](https://signalflow.dev/library/fft/fftlpf/), [FFTNoiseGate](https://signalflow.dev/library/fft/fftnoisegate/), [FFTPhaseVocoder](https://signalflow.dev/library/fft/fftphasevocoder/), [FFTTonality](https://signalflow.dev/library/fft/ffttonality/), [FFTZeroPhase](https://signalflow.dev/library/fft/fftzerophase/) |
| **Operators** | [Add](https://signalflow.dev/library/operators/add/), [AmplitudeToDecibels](https://signalflow.dev/library/operators/amplitudetodecibels/), [DecibelsToAmplitude](https://signalflow.dev/library/operators/decibelstoamplitude/), [Bus](https://signalflow.dev/library/operators/bus/), [ChannelArray](https://signalflow.dev/library/operators/channelarray/), [ChannelCrossfade](https://signalflow.dev/library/operators/channelcrossfade/), [ChannelMixer](https://signalflow.dev/library/operators/channelmixer/), [ChannelOffset](https://signalflow.dev/library/operators/channeloffset/), [ChannelSelect](https://signalflow.dev/library/operators/channelselect/), [Equal](https://signalflow.dev/library/operators/equal/), [NotEqual](https://signalflow.dev/library/operators/notequal/), [GreaterThan](https://signalflow.dev/library/operators/greaterthan/), [GreaterThanOrEqual](https://signalflow.dev/library/operators/greaterthanorequal/), [LessThan](https://signalflow.dev/library/operators/lessthan/), [LessThanOrEqual](https://signalflow.dev/library/operators/lessthanorequal/), [Modulo](https://signalflow.dev/library/operators/modulo/), [Abs](https://signalflow.dev/library/operators/abs/), [If](https://signalflow.dev/library/operators/if/), [Divide](https://signalflow.dev/library/operators/divide/), [FrequencyToMidiNote](https://signalflow.dev/library/operators/frequencytomidinote/), [MidiNoteToFrequency](https://signalflow.dev/library/operators/midinotetofrequency/), [Multiply](https://signalflow.dev/library/operators/multiply/), [Pow](https://signalflow.dev/library/operators/pow/), [RoundToScale](https://signalflow.dev/library/operators/roundtoscale/), [Round](https://signalflow.dev/library/operators/round/), [ScaleLinExp](https://signalflow.dev/library/operators/scalelinexp/), [ScaleLinLin](https://signalflow.dev/library/operators/scalelinlin/), [SelectInput](https://signalflow.dev/library/operators/selectinput/), [Subtract](https://signalflow.dev/library/operators/subtract/), [Sum](https://signalflow.dev/library/operators/sum/), [TimeShift](https://signalflow.dev/library/operators/timeshift/), [Sin](https://signalflow.dev/library/operators/sin/), [Cos](https://signalflow.dev/library/operators/cos/), [Tan](https://signalflow.dev/library/operators/tan/), [Tanh](https://signalflow.dev/library/operators/tanh/) |
| **Oscillators** | [Constant](https://signalflow.dev/library/oscillators/constant/), [Impulse](https://signalflow.dev/library/oscillators/impulse/), [LFO](https://signalflow.dev/library/oscillators/lfo/), [SawLFO](https://signalflow.dev/library/oscillators/sawlfo/), [SawOscillator](https://signalflow.dev/library/oscillators/sawoscillator/), [SineLFO](https://signalflow.dev/library/oscillators/sinelfo/), [SineOscillator](https://signalflow.dev/library/oscillators/sineoscillator/), [SquareLFO](https://signalflow.dev/library/oscillators/squarelfo/), [SquareOscillator](https://signalflow.dev/library/oscillators/squareoscillator/), [TriangleLFO](https://signalflow.dev/library/oscillators/trianglelfo/), [TriangleOscillator](https://signalflow.dev/library/oscillators/triangleoscillator/), [Wavetable](https://signalflow.dev/library/oscillators/wavetable/), [Wavetable2D](https://signalflow.dev/library/oscillators/wavetable2d/) |
| **Processors** | [Clip](https://signalflow.dev/library/processors/clip/), [Fold](https://signalflow.dev/library/processors/fold/), [Smooth](https://signalflow.dev/library/processors/smooth/), [WetDry](https://signalflow.dev/library/processors/wetdry/), [Wrap](https://signalflow.dev/library/processors/wrap/) |
| **Processors: Delays** | [AllpassDelay](https://signalflow.dev/library/processors/delays/allpassdelay/), [CombDelay](https://signalflow.dev/library/processors/delays/combdelay/), [OneTapDelay](https://signalflow.dev/library/processors/delays/onetapdelay/), [Stutter](https://signalflow.dev/library/processors/delays/stutter/) |
| **Processors: Distortion** | [Resample](https://signalflow.dev/library/processors/distortion/resample/), [SampleAndHold](https://signalflow.dev/library/processors/distortion/sampleandhold/), [Squiz](https://signalflow.dev/library/processors/distortion/squiz/), [WaveShaper](https://signalflow.dev/library/processors/distortion/waveshaper/) |
| **Processors: Dynamics** | [Compressor](https://signalflow.dev/library/processors/dynamics/compressor/), [Gate](https://signalflow.dev/library/processors/dynamics/gate/), [Maximiser](https://signalflow.dev/library/processors/dynamics/maximiser/), [RMS](https://signalflow.dev/library/processors/dynamics/rms/) |
| **Processors: Filters** | [BiquadFilter](https://signalflow.dev/library/processors/filters/biquadfilter/), [DCFilter](https://signalflow.dev/library/processors/filters/dcfilter/), [EQ](https://signalflow.dev/library/processors/filters/eq/), [MoogVCF](https://signalflow.dev/library/processors/filters/moogvcf/), [SVFilter](https://signalflow.dev/library/processors/filters/svfilter/) |
| **Processors: Panning** | [AzimuthPanner](https://signalflow.dev/library/processors/panning/azimuthpanner/), [ChannelPanner](https://signalflow.dev/library/processors/panning/channelpanner/), [SpatialPanner](https://signalflow.dev/library/processors/panning/spatialpanner/), [StereoBalance](https://signalflow.dev/library/processors/panning/stereobalance/), [StereoPanner](https://signalflow.dev/library/processors/panning/stereopanner/), [StereoWidth](https://signalflow.dev/library/processors/panning/stereowidth/) |
| **Sequencing** | [ClockDivider](https://signalflow.dev/library/sequencing/clockdivider/), [Counter](https://signalflow.dev/library/sequencing/counter/), [Euclidean](https://signalflow.dev/library/sequencing/euclidean/), [FlipFlop](https://signalflow.dev/library/sequencing/flipflop/), [ImpulseSequence](https://signalflow.dev/library/sequencing/impulsesequence/), [Index](https://signalflow.dev/library/sequencing/index/), [Latch](https://signalflow.dev/library/sequencing/latch/), [Sequence](https://signalflow.dev/library/sequencing/sequence/), [TriggerMult](https://signalflow.dev/library/sequencing/triggermult/), [TriggerRoundRobin](https://signalflow.dev/library/sequencing/triggerroundrobin/) |
| **Stochastic** | [Logistic](https://signalflow.dev/library/stochastic/logistic/), [PinkNoise](https://signalflow.dev/library/stochastic/pinknoise/), [RandomBrownian](https://signalflow.dev/library/stochastic/randombrownian/), [RandomChoice](https://signalflow.dev/library/stochastic/randomchoice/), [RandomCoin](https://signalflow.dev/library/stochastic/randomcoin/), [RandomExponentialDist](https://signalflow.dev/library/stochastic/randomexponentialdist/), [RandomExponential](https://signalflow.dev/library/stochastic/randomexponential/), [RandomGaussian](https://signalflow.dev/library/stochastic/randomgaussian/), [RandomImpulseSequence](https://signalflow.dev/library/stochastic/randomimpulsesequence/), [RandomImpulse](https://signalflow.dev/library/stochastic/randomimpulse/), [RandomUniform](https://signalflow.dev/library/stochastic/randomuniform/), [StochasticNode](https://signalflow.dev/library/stochastic/stochasticnode/), [WhiteNoise](https://signalflow.dev/library/stochastic/whitenoise/) |

## Contributors

Thanks to the following contributors:

- Greg White ([@gregwht](https://github.com/gregwht)): Examples, documentation and testing
- Arthur Carabott ([@acarabott](https://github.com/acarabott)), Tim Murray-Browne ([@timmb](https://github.com/timmb)): Windows support
- Dan Stowell ([@danstowell](https://github.com/danstowell)): Build improvements
- Marc Fargas ([@telenieko](https://github.com/telenieko)): Generous donation of the `signalflow` pypi namespace
- Ed Gillett: Generous donation of Linux build machine
- Sid Gudka: Generous donation of Windows build machine

## License

Use of the SignalFlow library with a Python interpreter, script or notebook is licensed under the free and permissive [MIT License](LICENSE.md).

Use of SignalFlow or its components outside of a Python environment (for example, embedded within a binary object) is subject to a separate agreement. Please [contact the author](https://danieljohnjones.com/contact/) to discuss.
