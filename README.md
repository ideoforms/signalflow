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
 * Initializing Sine with an array of frequencies creates a stereo output.
 *-----------------------------------------------------------------------*/
NodeRef sine = new Sine({ 440, 880 });

/*------------------------------------------------------------------------
 * Simple attack/sustain/release envelope with linear curves.
 *-----------------------------------------------------------------------*/
NodeRef env = new EnvelopeASR(0.01, 0.1, 0.5);

/*------------------------------------------------------------------------
 * Operator overloading: Modulate the sine wave's amplitude with the
 * output of the ASR envelope.
 *-----------------------------------------------------------------------*/
NodeRef ping = sine * env;

graph->add_output(ping);
graph->start();
graph->wait();
```

## Installation

### Dependencies

#### macOS

To install dependencies with Homebrew:

```
brew install libsndfile libsoundio gsl
```

#### Linux, Raspberry Pi

```
apt-get install git cmake g++ libasound2-dev libgsl0-dev libsndfile1-dev libsoundio-dev fftw3-dev
```

### Build (C++)

```
mkdir build
cd build
cmake ..
make -j8
```

### Build (Python)

```
python3 setup.py build
python3 setup.py test
python3 setup.py install
```

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

