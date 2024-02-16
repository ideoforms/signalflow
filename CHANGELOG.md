# CHANGELOG

## [v0.4.6](https://github.com/ideoforms/signalflow/tree/v0.4.6) (2024-02-16)

- Added `NearestNeighbours`, `KDTree` and `signalflow_analysis` package (including `AudioFeatureBuffer`) for audio feature analysis, latent space navigation and playback
- Added `signalflow_visualisation` package for visualising patch structures with NetworkX
- New nodes: `FFTContrast`, `FFTTransform`, `FFTMagnitudePhaseArray`, `FFTRandomPhase`, `TimeShift`
- Many documentation additions and Node examples (thanks to @gregwht)

## [v0.4.3](https://github.com/ideoforms/signalflow/tree/v0.4.3) (2023-11-12)

- Added Windows build support for Python
- Added `DCFilter` node

## [v0.4.2](https://github.com/ideoforms/signalflow/tree/v0.4.2) (2023-11-10)

- Added initial inline documentation for each Node class
- Added auto-generated method stubs for IDE inline docs and type hints
- Added initial example notebooks
- Added `signalflow_examples` downloader

## [v0.4.1](https://github.com/ideoforms/signalflow/tree/v0.4.0) (2023-11-05)

- New node: `ChannelCrossfade`
- `RectangularEnvelope`: Rename from `EnvelopeRect` for consistency with other `Envelope` classes
- `Node`: Add `is_playing` property
- `Node`: Refactor `play`/`stop` error handling to use `NodeAlreadyPlayingException`, `NodeNotPlayingException`, `CPUUsageAboveLimitException`
- `BufferPlayer`: Add `position` property
- `Buffer`: Add `filename` read-only property
- `SawLFO`, `SawOscillator`: Implement phase reset with `trigger`
- `Granulator`: Add support for negative playback rates, stereo input
- `signalflow_midi`: Add support for `MIDIManager` per-channel binding
- Add wheel for Python 3.12

## [v0.4.0](https://github.com/ideoforms/signalflow/tree/v0.4.0) (2023-05-28)

- Implemented complete Python API
- Migrated to CMake for build
- Migrated to GitHub Actions for CI
- Introduced initial Windows support
- Naming overhaul and standardisation
- Added `AudioGraphConfig`, and support for loading config from file and environmental variables
- Added multiple `Node` subclasses for spatial panning
- Added `signalflow` command-line utility
- Added `signalflow_midi` helper package

## [v0.3.1](https://github.com/ideoforms/signalflow/tree/v0.3.1) (2018-12-31)

- Node: Ability to stop playback and remove from graph [\#46](https://github.com/ideoforms/signalflow/issues/46)
- Unify code indentation to use spaces throughout [\#76](https://github.com/ideoforms/signalflow/issues/76)
- Add support for initializing NodeRef/Multiplex with std::vector [\#75](https://github.com/ideoforms/signalflow/issues/75)
- Monitor: Add support for multichannel monitoring [\#74](https://github.com/ideoforms/signalflow/issues/74)

## [v0.3.0](https://github.com/ideoforms/signalflow/tree/v0.3.0) (2018-07-15)

- BufferRef [\#66](https://github.com/ideoforms/signalflow/issues/66)
- Example: audio-write-example [\#59](https://github.com/ideoforms/signalflow/issues/59)
- Buffer: Save to wav file [\#58](https://github.com/ideoforms/signalflow/issues/58)
- Support for named triggers [\#56](https://github.com/ideoforms/signalflow/issues/56)
- Trigger-based random number generation [\#49](https://github.com/ideoforms/signalflow/issues/49)
- Non-realtime processing [\#16](https://github.com/ideoforms/signalflow/issues/16)
- Fixed: Multiplex cannot be serialised [\#55](https://github.com/ideoforms/signalflow/issues/55)
- Node: MouseX/MouseY [\#65](https://github.com/ideoforms/signalflow/issues/65)
- Integrate with Travis CI [\#54](https://github.com/ideoforms/signalflow/issues/54)
- Generate visualisation of synthesis graph structure [\#53](https://github.com/ideoforms/signalflow/issues/53)
- Naming review [\#44](https://github.com/ideoforms/signalflow/issues/44)

## [v0.2.0](https://github.com/ideoforms/signalflow/tree/v0.2.0) (2016-04-17)

- SynthRegistry [\#51](https://github.com/ideoforms/signalflow/issues/51)
- Node: Support for constant property values [\#39](https://github.com/ideoforms/signalflow/issues/39)
- Multiplex: Support for std::initializer\_list  [\#37](https://github.com/ideoforms/signalflow/issues/37)
- Buffer: Fill buffer from a lambda function [\#35](https://github.com/ideoforms/signalflow/issues/35)
- Buffer: Waveshaper buffers [\#33](https://github.com/ideoforms/signalflow/issues/33)
- Node: Waveshaper [\#27](https://github.com/ideoforms/signalflow/issues/27)
- JSON parsing to load serialised synth graphs [\#17](https://github.com/ideoforms/signalflow/issues/17)
- SynthDef: Support for named inputs and outputs [\#4](https://github.com/ideoforms/signalflow/issues/4)
- Support for non-44k samplerates [\#2](https://github.com/ideoforms/signalflow/issues/2)
- Buffer recording [\#1](https://github.com/ideoforms/signalflow/issues/1)
- Fixed: Add node-constant operations where node is the RHS operand [\#30](https://github.com/ideoforms/signalflow/issues/30)
- Fixed: Prevent a node from being stepped multiple times per tick [\#29](https://github.com/ideoforms/signalflow/issues/29)
- Fixed: Rename channels\_out → num\_output\_channels for consistency [\#23](https://github.com/ideoforms/signalflow/issues/23)
- Further examples [\#48](https://github.com/ideoforms/signalflow/issues/48)
- Node: MoogVCF [\#43](https://github.com/ideoforms/signalflow/issues/43)
- Node: Linear scale [\#41](https://github.com/ideoforms/signalflow/issues/41)
- Node: Triangle [\#26](https://github.com/ideoforms/signalflow/issues/26)
- Node: Saw [\#25](https://github.com/ideoforms/signalflow/issues/25)
- Update basic nodes to handle multi-channel inputs [\#24](https://github.com/ideoforms/signalflow/issues/24)
- Node: EQ [\#19](https://github.com/ideoforms/signalflow/issues/19)

## [v0.1.1](https://github.com/ideoforms/signalflow/tree/v0.1.1) (2016-03-23)

- Incorporate JSON parser library [\#7](https://github.com/ideoforms/signalflow/issues/7)
- Correct loading of multichannel buffers [\#18](https://github.com/ideoforms/signalflow/issues/18)

## [v0.0.1](https://github.com/ideoforms/signalflow/tree/v0.0.1) (2016-01-01)

- Basic framework with `Node` and `Graph` classes
- Automatic reference counting with `NodeRef` as a subclass of `std::shared_ptr`
- Synthesis nodes: `Sine`, `Square`, `Granulator`, `Wavetable`
- Control nodes: `Tick`, `Line`, `ASR`
- Effects nodes: `Delay`, `Gate`, `Pan`, `Resample`, `Width`
- Chance nodes: `RandomImpulse`, `Noise`
- `Buffer` object to store fixed buffers of sample data
- `RingBuffer` for circular buffering
- Global `Registry` to store and instantiate `Node` objects by identifier
- Fast Fourier Transform (`FFT`) and inverse FFT (`IFFT`) nodes with Accelerate optimisation
- Operator overloading for node addition, multiplication, addition and subtraction
- Node output monitoring
- Cross-platform I/O with `libsoundio` 
- Audio file I/O with `libsndfile`
- waf-based build system
- Raspberry Pi support
