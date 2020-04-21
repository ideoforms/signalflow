# signal

![ci](https://github.com/ideoforms/signal/workflows/ci/badge.svg)

Signal is an audio synthesis engine designed for structural clarity, lightness of code, and concise expression of complex ideas. It is built with modern standards, taking advantage of the powerful expressive capabilities of c++11 and json.

Signal is in alpha status. Interfaces may be subject to change. 

## Example

```cpp
#include <signal/signal.h>
using namespace libsignal;

AudioGraphRef graph = new AudioGraph();

/*------------------------------------------------------------------------
 * A SynthTemplate constructs a reusable synthesis graph.
 * Objects whose names end in Ref are std::shared_ptr smart pointers,
 * for automatic memory management.
 *-----------------------------------------------------------------------*/
SynthTemplateRef tmp = new SynthTemplate("ping");

/*------------------------------------------------------------------------
 * Initializing Sine with an array of frequencies creates a stereo output.
 *-----------------------------------------------------------------------*/
NodeRef sine = tmp->add_node(new Sine({ 440, 880 }));

/*------------------------------------------------------------------------
 * Simple attack/sustain/release envelope with linear curves.
 *-----------------------------------------------------------------------*/
NodeRef env = tmp->add_node(new ASR(0.01, 0.1, 0.5));

/*------------------------------------------------------------------------
 * Operator overloading: Modulate the sine wave's amplitude with the
 * output of the ASR envelope.
 *-----------------------------------------------------------------------*/
NodeRef ping = tmp->add_node(sine * env);

/*------------------------------------------------------------------------
 * Single-tap delay line with feedback.
 *-----------------------------------------------------------------------*/
NodeRef delay = tmp->add_node(new Delay(ping, 0.5, 0.5));

/*------------------------------------------------------------------------
 * A SynthTemplate must have a single output node. 
 *-----------------------------------------------------------------------*/
tmp->set_output(delay);

/*------------------------------------------------------------------------
 * Instantiate the synth and route it to the audio output,
 * then start the AudioGraph and run forever.
 *-----------------------------------------------------------------------*/
SynthRef synth = new Synth(tmp);
graph->add_output(synth);
graph->start();
graph->wait();
```

## Installation

#### macOS

To build with Homebrew dependencies:

```
brew install libsndfile libsoundio gsl
./waf
```

#### Linux (Ubuntu 14)

* `apt-get install git cmake g++ libasound2-dev libgsl0-dev libsndfile1-dev`
* Install [libsoundio](http://libsound.io/)
* `./waf`

#### Raspberry Pi

Requires Raspbian jessie (for `g++ 4.9`).

* `apt-get install git cmake libasound2-dev libgsl0-dev libsndfile1-dev`
* Install [libsoundio](https://github.com/andrewrk/libsoundio) (latest GitHub version required)
* `./waf`

## Examples

See [examples](examples) for a number of example programs.

To run an example:
```
cd build
./hello-world
```

## License

For non-commercial use, Signal is available under the terms of the [GPL v3](http://www.gnu.org/licenses/gpl-3.0.en.html).

For commercial use, please [contact the author](http://erase.net/contact).

