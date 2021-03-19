#!/usr/bin/env python3

#------------------------------------------------------------------------
# MIDI Control for SignalFlow
#------------------------------------------------------------------------
from signalflow import *
import threading
import mido

class MIDIManager:
    shared_manager = None

    def __init__(self, device_name=None):
        self.input = mido.open_input(device_name)
        self.input.callback = self.handle_message

        self.voice_class = None
        self.voice_class_kwargs = None
        self.notes = [None] * 128
        self.note_handler = None
        self.control_handlers = [None] * 128
        if MIDIManager.shared_manager is None:
            MIDIManager.shared_manager = self

    def handle_message(self, message):
        if message.type == 'control_change':
            self.on_control_change(message.control, message.value)
        elif message.type == 'note_on':
            if self.voice_class:
                voice = self.voice_class(midi_note_to_frequency(message.note), **self.voice_class_kwargs)
                voice.play()
                voice.auto_free = True
                self.notes[message.note] = voice
        elif message.type == 'note_off':
            if self.notes[message.note]:
                self.notes[message.note].set_input("gate", 0)

    def get_shared_manager():
        if MIDIManager.shared_manager is None:
            MIDIManager.shared_manager = MIDIManager()
        return MIDIManager.shared_manager

    def set_voice_class(self, cls, **kwargs):
        self.voice_class = cls
        self.voice_class_kwargs = kwargs

    def add_note_handler(self, handler):
        self.note_handler = handler

    def add_control_handler(self, control, handler):
        assert self.control_handlers[control] is None
        self.control_handlers[control] = handler

    def on_control_change(self, control, value):
        if self.control_handlers[control] is not None:
            self.control_handlers[control].on_change(value)

class MIDIControl (Patch):
    def __init__(self, control, range_min, range_max, initial=None, mode="relative", manager=None, curve="linear"):
        super().__init__()
        if manager is None:
            manager = MIDIManager.get_shared_manager()
        self.value = self.add_input("value")
        self.value_smoothed = Smooth(self.value, 0.999)
        self.set_output(self.value_smoothed)
        self.control = control
        self.range_min = range_min
        self.range_max = range_max
        self.curve = curve
        if initial:
            if self.curve == "exponential":
                self.value = scale_exp_lin(initial, range_min, range_max, 0, 1)
            elif self.curve == "linear":
                self.value = scale_lin_lin(initial, range_min, range_max, 0, 1)
        else:
            self.value = 0.5
        self.update()
        self.mode = mode

        manager.add_control_handler(control, self)

    def on_change(self, value):
        if self.mode == "absolute":
            self.value = value / 127.0
        else:
            change = (value - 64) / 127.0
            self.value += change
            if self.value < 0:
                self.value = 0
            if self.value > 1:
                self.value = 1
        self.update()

    def update(self):
        if self.curve == "exponential":
            value_scaled = scale_lin_exp(self.value, 0, 1, self.range_min, self.range_max)
        elif self.curve == "linear":
            value_scaled = scale_lin_lin(self.value, 0, 1, self.range_min, self.range_max)
        self.set_input("value", value_scaled)

