---
layout: default
title: signal
---

<div class="intro">
signal is an audio synthesis engine designed for structural clarity, lightness of code, and concise expression of complex ideas. It is built upon on modern standards, taking advantage of the powerful expressive capabilities of c++11 and json.
</div>

```cpp
GraphRef graph = new Graph();

StructRef structure = new Structure("ping");
NodeRef sine = structure->add_node(new Sine(440));
NodeRef env = structure->add_node(new ASR(0.01, 0.1, 5.0));
NodeRef pan = structure->add_node(new Pan(2, env * sine, 0.5));
structure->set_output(pan);

SynthRef synth = new Synth(structure);
graph->output->add_input(synth->output);
graph->run();
```

{::options parse_block_html="true" /}
<div class="row">
<div class="box">
### Built on familiar protocols
Where possible, existing standards are used throughout: JSON to describe object graphs, REST/HTTP for client-server comms.
</div>

<div class="box">
### Cross-platform
Tested on OS X, iOS, Linux and Raspberry Pi (Jessie). Cross-platform I/O courtesy of [libsoundio](http://libsound.io/).
</div>

<div class="box">
### Free of memory allocation headaches
Key objects are transparently instantiated via `shared_ptr` reference counters, meaning less manual memory allocation.
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

**What it can do now:** Cross-platform audio I/O (OS X, iOS, Linux, rpi), Oscillators and wavetables, Sample playback, Granular synthesis, Multichannel expansion, Node operations via standard operators, Synth graph templates, Reference-counted memory management

**What it will do in the future:** RESTful client-server architecture, Python interface, Convolution reverb, Binaural spatialisation with HRTF, System install as a shared library, Non-realtime processing, Audio analysis via Vamp, Sound-source positioning and multichannel spatialisation

**What it will not do:** Graphical user interface, Timelines and event scheduling, Web audio 

