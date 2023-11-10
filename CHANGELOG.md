# CHANGELOG

## v0.4.2 (2023-11-10)

- Add initial inline documentation for each Node class
- Add auto-generated method stubs for IDE inline docs and type hints
- Add initial example notebooks
- Add `signalflow_examples` downloader

## v0.4.1 (2023-11-05)

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

## v0.4.0 (2023-05-28)

- Implemented complete Python API
- Migrated to CMake for build
- Migrated to GitHub Actions for CI
- Introduced initial Windows support
- Naming overhaul and standardisation
- Added `AudioGraphConfig`, and support for loading config from file and environmental variables
- Added multiple `Node` subclasses for spatial panning
- Added `signalflow` command-line utility
- Added `signalflow_midi` helper package

## [v0.3.1](https://github.com/ideoforms/signal/tree/v0.3.1) (2018-12-31)

**New features:**

- Node: Ability to stop playback and remove from graph [\#46](https://github.com/ideoforms/signal/issues/46)

**Closed issues:**

- Unify code indentation to use spaces throughout [\#76](https://github.com/ideoforms/signal/issues/76)
- Add support for initializing NodeRef/Multiplex with std::vector [\#75](https://github.com/ideoforms/signal/issues/75)
- Monitor: Add support for multichannel monitoring [\#74](https://github.com/ideoforms/signal/issues/74)

## [v0.3.0](https://github.com/ideoforms/signal/tree/v0.3.0) (2018-07-15)

**New features:**

- BufferRef [\#66](https://github.com/ideoforms/signal/issues/66)
- Example: audio-write-example [\#59](https://github.com/ideoforms/signal/issues/59)
- Buffer: Save to wav file [\#58](https://github.com/ideoforms/signal/issues/58)
- Support for named triggers [\#56](https://github.com/ideoforms/signal/issues/56)
- Trigger-based random number generation [\#49](https://github.com/ideoforms/signal/issues/49)
- Non-realtime processing [\#16](https://github.com/ideoforms/signal/issues/16)

**Fixed bugs:**

- Multiplex cannot be serialised [\#55](https://github.com/ideoforms/signal/issues/55)

**Closed issues:**

- Node: MouseX/MouseY [\#65](https://github.com/ideoforms/signal/issues/65)
- Integrate with Travis CI [\#54](https://github.com/ideoforms/signal/issues/54)
- Generate visualisation of synthesis graph structure [\#53](https://github.com/ideoforms/signal/issues/53)
- Naming review [\#44](https://github.com/ideoforms/signal/issues/44)

## [v0.2.0](https://github.com/ideoforms/signal/tree/v0.2.0) (2016-04-17)

**New features:**

- SynthRegistry [\#51](https://github.com/ideoforms/signal/issues/51)
- Node: Support for constant property values [\#39](https://github.com/ideoforms/signal/issues/39)
- Multiplex: Support for std::initializer\_list  [\#37](https://github.com/ideoforms/signal/issues/37)
- Buffer: Fill buffer from a lambda function [\#35](https://github.com/ideoforms/signal/issues/35)
- Buffer: Waveshaper buffers [\#33](https://github.com/ideoforms/signal/issues/33)
- Node: Waveshaper [\#27](https://github.com/ideoforms/signal/issues/27)
- JSON parsing to load serialised synth graphs [\#17](https://github.com/ideoforms/signal/issues/17)
- SynthDef: Support for named inputs and outputs [\#4](https://github.com/ideoforms/signal/issues/4)
- Support for non-44k samplerates [\#2](https://github.com/ideoforms/signal/issues/2)
- Buffer recording [\#1](https://github.com/ideoforms/signal/issues/1)

**Fixed bugs:**

- Add node-constant operations where node is the RHS operand [\#30](https://github.com/ideoforms/signal/issues/30)
- Prevent a node from being stepped multiple times per tick [\#29](https://github.com/ideoforms/signal/issues/29)
- Rename channels\_out → num\_output\_channels for consistency [\#23](https://github.com/ideoforms/signal/issues/23)

**Closed issues:**

- Further examples [\#48](https://github.com/ideoforms/signal/issues/48)
- Node: MoogVCF [\#43](https://github.com/ideoforms/signal/issues/43)
- Node: Linear scale [\#41](https://github.com/ideoforms/signal/issues/41)
- Node: Triangle [\#26](https://github.com/ideoforms/signal/issues/26)
- Node: Saw [\#25](https://github.com/ideoforms/signal/issues/25)
- Update basic nodes to handle multi-channel inputs [\#24](https://github.com/ideoforms/signal/issues/24)
- Node: EQ [\#19](https://github.com/ideoforms/signal/issues/19)

## [v0.1.1](https://github.com/ideoforms/signal/tree/v0.1.1) (2016-03-23)

**New features:**

- Incorporate JSON parser library [\#7](https://github.com/ideoforms/signal/issues/7)

**Fixed bugs:**

- Correct loading of multichannel buffers [\#18](https://github.com/ideoforms/signal/issues/18)

## v0.0.1 (2016-01-01)

**New features:**

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


\* *This Change Log was automatically generated by [github_changelog_generator](https://github.com/skywinder/Github-Changelog-Generator)*
