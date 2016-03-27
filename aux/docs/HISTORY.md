## v0.0.1 (2016-01-01)

**New features:**

- Basic framework with `Node` and `Graph` classes
- Automatic reference counting with `NodeRef` as a subclass of `std::shared_ptr`
- Synthesis nodes: `Sine`, `Square`, `Granulator`, `Wavetable`
- Control nodes: `Tick`, `Line`, `ASR`
- Effects nodes: `Delay`, `Gate`, `Pan`, `Resample`, `Width`
- Chance nodes: `Dust`, `Noise`
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
