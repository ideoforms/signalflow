# Troubleshooting: NodeNotPlayingException

## Example

```
signalflow.NodeNotPlayingException: Node cannot be played as it is already
playing
```

## Description

This exception occurs when `stop()` is called on a `Node` object that has not yet been played.      

## Solution

Calling `stop()` on a `Node` object that is not connected to an output is invalid, and should be avoided.

If you want to check whether a `Node` is connected before stopping it, you can check the boolean `is_playing` property.