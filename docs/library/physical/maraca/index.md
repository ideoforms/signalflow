title: Maraca node documentation
description: Maraca: Physically-inspired model of a maraca.

[Reference library](../../index.md) > [Physical](../index.md) > [Maraca](index.md)

# Maraca

```python
Maraca(num_beans=64, shake_decay=0.99, grain_decay=0.99, shake_duration=0.02, shell_frequency=12000, shell_resonance=0.9, clock=None, energy=None)
```

Physically-inspired model of a maraca. 

 Parameters: - `num_beans`: The number of simulated beans in the maraca (1-1024) - `shake_decay`: Decay constant for the energy injected per shake - `grain_decay`: Decay constant for the energy created per bean collision - `shake_duration`: Duration of each shake action, milliseconds - `shell_frequency`: Resonant frequency of the maraca's shell, hertz - `shell_resonance`: Resonanc of the maraca's shell (0-1) - `clock`: If specified, triggers shake actions - `energy`: If specified, injects energy into the maraca 

 From Cook (1997), \"Physically Informed Sonic Modeling (PhISM): Synthesis of Percussive Sounds\", Computer Music Journal.

