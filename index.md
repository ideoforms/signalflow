---
layout: default
title: signal
---

<div class="intro">
signal is an audio synthesis engine designed for structural clarity, lightness of code, and concise expression of complex ideas. It is built with modern standards, taking advantage of the powerful expressive capabilities of c++11 and json.
</div>

```cpp
AudioGraphRef graph = new AudioGraph();

NodeRef sine = new Sine(440);
NodeRef env = new ASR(0.01, 0.1, 5.0);
NodeRef stereo = new Pan(2, env * sine, 0.5);

graph->add_output(stereo);
graph->start();
```

{::options parse_block_html="true" /}
<div class="row">
<div class="box">
### Built on familiar protocols
Uses common standards throughout, with audio graphs serialised using JSON.
</div>

<div class="box">
### Cross-platform
Runs on macOS, iOS, Linux x86 and Raspberry Pi. Cross-platform audio I/O courtesy of [libsoundio](http://libsound.io/).
</div>

<div class="box">
### Free of memory allocation headaches
In C++, objects are handled with `shared_ptr` reference counters, minimising the need for manual memory management.
</div>

</div>
<div class="row">

<div class="box">
### Concise expression of complex ideas
Instantiate an audio-generating node in a single line of C++. Combine and modulate nodes with the regular <code>+-*/</code> operators.
</div>


<div class="box">
### Processor efficient
On iOS and OSX, uses Accelerate for hardware-accelerated FFT and vector optimisations.
</div>

<div class="box">
### Modular and hackable
Designed for clarity, with modularised and accessible code.
</div>

</div>

---

**What it can do now:** Cross-platform audio I/O (OS X, iOS, Linux, rpi), Oscillators and wavetables, Sample playback, Granular synthesis, Multichannel expansion, Node operations via standard operators, Synth graph templates, Reference-counted memory management, Non-realtime processing, Audio analysis via [Vamp](http://www.vamp-plugins.org/)

**What it will do in the future:** RESTful client-server architecture, Python interface, Convolution reverb, Binaural spatialisation with HRTF, System install as a shared library, Sound-source positioning and multichannel spatialisation
