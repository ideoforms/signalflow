# Signal Examples

**[audio-in-example.cpp](audio-in-example.cpp)**:
Takes audio input from the default input device, records it to
a short buffer, and plays it back at variable rates.

**[audio-write-example.cpp](audio-write-example.cpp)**:
Demonstrates recording audio input (or any other synthesis node)
to a buffer, and saving the output to disk as a .wav file.

**[fft-lpf-example.cpp](fft-lpf-example.cpp)**:
Performs an FFT on an incoming signal, then passes it through
a frequency-domain brick wall filter, zeroing any bins beyond
the specified cutoff frequency.

**[granulator-example.cpp](granulator-example.cpp)**:
Demonstrates granular synthesis upon an audio buffer, with randomly
modulated position and length, and a user-specified grain envelope.

**[hello-world.cpp](hello-world.cpp)**:
The canonical 440hz sine wave example.

**[index-example.cpp](index-example.cpp)**:
Demonstrates using the `Index` node to retrieve indexed items
from a `std::vector`. Note that the first argument to Index is a
static property, not a node.

**[json-load-example.cpp](json-load-example.cpp)**:
Demonstrates loading a synth spec from a JSON graph description.
Optional pathname to a JSON file can be passed in argv.

**[sine-field-example.cpp](sine-field-example.cpp)**:
An array of delayed sine pings.

**[spooky-wobble.cpp](spooky-wobble.cpp)**:
Tape-wobble effect with crackle.

**[supersaw-example.cpp](supersaw-example.cpp)**:
Demonstrates using Signal's multichannel expansion and mixdown
to create complex harmonics.

**[synth-template-example.cpp](synth-template-example.cpp)**:
Demonstrates the creation of a reusable graph of nodes, that can
be subsequently replicated for polyphonic output.

**[trigger-example.cpp](trigger-example.cpp)**:
Node triggers are discrete events that trigger a given behaviour
within a node. This example demonstrates using a trigger to
periodically reset the position of an envelope node.

**[waveshaper-example.cpp](waveshaper-example.cpp)**:
Demonstrates constructing a waveshaper buffer from a lambda function,
used to dynamically alter the timbre of an oscillator.

**[wavetable-example.cpp](wavetable-example.cpp)**:
Demonstrates using the `Wavetable` oscillator to generate periodic
waveforms from a fixed audio buffer.

