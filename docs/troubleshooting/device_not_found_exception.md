# Troubleshooting: DeviceNotFoundException

## Example

```
signalflow.DeviceNotFoundException: Could not find device name: Scarlett 2i2
```

## Description

This exception occurs because the audio device specified in the configuration could not be found, or no audio devices at all could be found.     

## Solution

Check that the device name that you have selected exists, and is connected to your computer and powered on.

 - SignalFlow selects its output device based on the device that you have selected in your `~/.signalflow/config`, in the environmental variable `SIGNALFLOW_OUTPUT_DEVICE_NAME` (or `SIGNALFLOW_INPUT_DEVICE_NAME` for input devices), or in the `config` property passed to the `AudioGraph`
 - A list of all available devices can be found by running the terminal command `signalflow list-output-device-names`
```
[audio]
output_buffer_size = 8192
```
