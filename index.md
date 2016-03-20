---
layout: default
title: Test
---

## Hello, world

This is a test, in **markdown**.

```cpp
Graph *graph = new Graph();

SynthDef *def = new SynthDef("test");
UnitRef sine = def->add_node(new Sine(440));
UnitRef env = def->add_node(new ASR(0.01, 0.1, 5.0));
UnitRef pan = def->add_node(new Pan(2, env * sine, 0.5));
def->set_output(pan);

SynthRef synth = new Synth(def);
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
### No memory allocation headaches
Key objects are transparently instantiated via `shared_ptr` reference counters, meaning much less manual memory allocation.
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
