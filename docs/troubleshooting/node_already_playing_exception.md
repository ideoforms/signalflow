# Troubleshooting: NodeAlreadyPlayingException

## Example

```
signalflow.NodeAlreadyPlayingException: Node cannot be played as it is already
playing
```

## Description

This exception occurs when `play()` is called on a `Node` or `Patch` object that is already playing.      

## Solution

Calling `play()` on a `Node` object simply connects the output of that node to the output endpoint of the audio graph, which means that frames will be requested from it in future audio I/O blocks.

Calling `play()` a second time therefore has no effect, because the `Node` is already connected. 

If you want to play a particular sound twice, you should instead create two instances of the `Node` or `Patch`, and play them individually. So instead of:

```python
ping = Ping()
ping.play()
time.sleep(1)
ping.play() # incorrect 
```

You should write:

```python
ping = Ping()
ping.play()
time.sleep(1)
ping = Ping() # create and play a new Ping 
ping.play() 
```