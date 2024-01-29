# Buffers

A `Buffer` is an area of memory that stores single-channel or multi-channel data, which may represent an audio waveform or any other type of signal. 

 - A Buffer can be [created](creating.md) from a sound file, an array of samples, a specified dimension, or the result of a function
 - A Buffer can be [saved to a sound file](exporting.md)
 - A Buffer can be [passed to a Node or Patch](input.md) as an input
 - Buffer [sample access](access.md) can be performed by get/set/fill methods, or directly as a numpy array 
 - Buffers can be modified, combined and queried with standard [arithmetic operators](operators.md)
 - Buffers can be queried for a number of [properties](properties.md), including interpolation modes and total memory usage
 - _TODO_: Different Buffer subclasses exist for specific operations, including `Buffer2D`, `WaveshaperBuffer` and `EnvelopeBuffer`
 - _TODO_: Playing a buffer, including sample rate conversion and interpolation
 - _TODO_: Recording and rendering audio into a Buffer
 