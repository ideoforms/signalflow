title: MouseY node documentation
description: MouseY: Outputs the normalised cursor Y position, from 0 to 1. Currently only supported on macOS.

[Reference library](../../index.md) > [Control](../index.md) > [MouseY](index.md)

# MouseY

```python
MouseY()
```

Outputs the normalised cursor Y position, from 0 to 1. Currently only supported on macOS.

### Examples

```python

#-------------------------------------------------------------------------------
# Using the MouseY position to change the frequency of an oscillator.
#-------------------------------------------------------------------------------
frequency = MouseY() * 1000
osc = TriangleOscillator(frequency)
osc.play()

```

```python

#-------------------------------------------------------------------------------
# A simple wobbling synthesiser controlled using the mouse. When the mouse is 
# clicked, as detected by MouseDown(), an LFO is activated and affects the 
# oscillator's frequency. MouseX position changes the rate of the LFO. MouseY 
# position changes the upper frequency limit, affecting pitch.
#-------------------------------------------------------------------------------
rate = MouseX() * 10
upper_limit = MouseY() * 1500
lfo = SineLFO(rate, 100, upper_limit)
frequency = If(MouseDown(), lfo, 100)
osc = TriangleOscillator(frequency)
osc.play()

```

