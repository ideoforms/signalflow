# Buffer properties

A `Buffer` has a number of read-only properties which can be used to query its status at a given moment in time.

| Property           | Type  | Description                                                                        |
|--------------------|-------|------------------------------------------------------------------------------------|
| num_frames         | int   | The number of frames (samples) in the buffer.                                      |
| channels           | int   | The number of channels in the buffer.                                              |
| sample_rate        | int   | The sample rate of the buffer.                                                     |
| duration           | float | The duration of the buffer, in seconds.                                            |
| filename           | str   | If the buffer has been loaded from/saved to a file, the absolute path to the file. | 
| interpolation_mode | str   | The interpolation mode of the buffer. [LINK]                                       |

---
