title: MouseDown node documentation
description: MouseDown: Outputs 1 if the left mouse button is down, 0 otherwise. Currently only supported on macOS.

[Reference library](../../index.md) > [Control](../index.md) > [MouseDown](index.md)

# MouseDown

```python
MouseDown(button_index=0)
```

Outputs 1 if the left mouse button is down, 0 otherwise. Currently only supported on macOS.

### Examples

```python

#-------------------------------------------------------------------------------
# When the left mouse button is clicked, as detected by MouseDown(), an LFO is 
# applied to the oscillator's frequency.
#-------------------------------------------------------------------------------
lfo = SineLFO(5, 100, 600)
frequency = If(MouseDown(), lfo, 100)
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

