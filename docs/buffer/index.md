# Buffer

!!! warning
    This documentation is a work-in-progress and may have sections that are missing or incomplete.

A `Buffer` is an allocated area of memory that can be used to store single-channel or multi-channel data, which may represent an audio waveform or any other type of signal. 

 - A Buffer can be created from a sound file, an array of samples, or with an empty contents
 - A Buffer can be passed to a Node or Patch as an input
 - A Buffer can be exported to a sound file
 - A Buffer's data can be directly accessed in memory as a numpy array, or by get/set methods
 - The contents of a buffer can be combined with arithmetic operators
 - Properties
 - Buffer interpolation modes
 - 2D buffers
 - Buffer applications: Sample recording and playback, control recording and playback, envelopes, waveshapers 
 - The total Buffer memory usage can be queried 
