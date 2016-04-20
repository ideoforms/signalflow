# signal

[![Build Status](https://travis-ci.org/ideoforms/signal.svg?branch=master)](https://travis-ci.org/ideoforms/signal)

Signal is an audio synthesis engine designed for structural clarity, lightness of code, and concise expression of complex ideas. It is built with modern standards, taking advantage of the powerful expressive capabilities of c++11 and json.

As of April 2016, Signal is in alpha status. Its codebase is under frequent development, and its interfaces may be subject to sudden change. 

## Installation

#### OS X

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

## License

For non-commercial use, Signal is available under the terms of the [GPL v3](http://www.gnu.org/licenses/gpl-3.0.en.html).

For commercial use, please [contact the author](http://erase.net/contact).

