#!/usr/bin/env python3

# ------------------------------------------------------------------------
# MIDI Control for SignalFlow
# ------------------------------------------------------------------------
from signalflow import *
import configparser
import logging
import mido
import os

logger = logging.getLogger(__name__)


class MIDIManager:
    shared_manager = None

    def __init__(self, device_name: str = None, channel: int = None):
        if device_name is None:
            if os.getenv("SIGNALFLOW_MIDI_OUTPUT_DEVICE_NAME") is not None:
                device_name = os.getenv("SIGNALFLOW_MIDI_OUTPUT_DEVICE_NAME")
            else:
                config_path = os.path.expanduser("~/.signalflow/config")
                parser = configparser.ConfigParser()
                parser.read(config_path)
                try:
                    # --------------------------------------------------------------------------------
                    # configparser includes quote marks in its values, so strip these out.
                    # --------------------------------------------------------------------------------
                    device_name = parser.get(section="midi", option="input_device_name")
                    device_name = device_name.strip('"')
                except configparser.NoOptionError:
                    pass
                except configparser.NoSectionError:
                    pass

        self.input = mido.open_input(device_name)
        self.channel = channel
        self.input.callback = self.handle_message

        self.voice_class = None
        self.voice_class_kwargs = None
        self.notes = [None] * 128
        self.note_handlers = [[] for _ in range(128)]
        self.control_handlers = [[] for _ in range(128)]
        self.control_values = [0] * 128
        self.channel_handlers = [[] for _ in range(16)]
        if MIDIManager.shared_manager is None:
            MIDIManager.shared_manager = self

        logger.info("Opened MIDI input device: %s" % self.input.name)

    def handle_message(self, message):
        if self.channel is not None and message.channel != self.channel:
            return

        if message.type == 'control_change':
            logger.debug("Received MIDI control change: control %d, value %d" % (message.control, message.value))
            self.on_control_change(message.control, message.value)
        elif message.type == 'note_on':
            logger.debug("Received MIDI note on: note %d, velocity %d" % (message.note, message.velocity))
            if self.voice_class:
                voice = self.voice_class(frequency=midi_note_to_frequency(message.note),
                                         amplitude=message.velocity / 127,
                                         **self.voice_class_kwargs)
                voice.play()
                voice.auto_free = True
                self.notes[message.note] = voice
            if self.note_handlers[message.note]:
                self.note_handlers[message.note](message.note, message.velocity)
        elif message.type == 'note_off':
            logger.debug("Received MIDI note off: note %d" % (message.note))
            if self.notes[message.note]:
                self.notes[message.note].set_input("gate", 0)

        try:
            channel = message.channel
            for handler in self.channel_handlers[channel]:
                handler.handle_message(message)
        except AttributeError:
            pass

    @classmethod
    def get_shared_manager(cls):
        if MIDIManager.shared_manager is None:
            MIDIManager.shared_manager = MIDIManager()
        return MIDIManager.shared_manager

    def set_voice_patch(self, cls, **kwargs):
        self.voice_class = cls
        self.voice_class_kwargs = kwargs

    def add_note_handler(self, note, handler):
        self.note_handlers[note] = handler

    def add_control_handler(self, control, handler):
        self.control_handlers[control].append(handler)

    def on_control_change(self, control, value):
        self.control_values[control] = value
        for handler in self.control_handlers[control]:
            handler.on_change(value)

    def get_control_value(self, control):
        return self.control_values[control]

    def add_channel_handler(self, channel, handler):
        self.channel_handlers[channel].append(handler)

    def remove_channel_handler(self, channel, handler):
        self.channel_handlers[channel].remove(handler)


class MIDIControl(Patch):
    def __init__(self, control, range_min, range_max, initial=None, mode="absolute", manager=None, curve="linear"):
        super().__init__()
        assert mode in ["absolute", "relative"]
        if manager is None:
            manager = MIDIManager.get_shared_manager()
        self.value = self.add_input("value")
        self.value_smoothed = Smooth(self.value, 0.999)
        self.set_output(self.value_smoothed)
        self.control = control
        self.range_min = range_min
        self.range_max = range_max
        self.curve = curve
        if initial is not None:
            if self.curve == "exponential":
                self._value_norm = scale_exp_lin(initial, range_min, range_max, 0, 1)
            elif self.curve == "linear":
                self._value_norm = scale_lin_lin(initial, range_min, range_max, 0, 1)
        else:
            self._value_norm = 0.5
        self.update()
        self.mode = mode

        manager.add_control_handler(control, self)

    def on_change(self, value):
        if self.mode == "absolute":
            self._value_norm = value / 127.0
        elif self.mode == "relative":
            if value > 64:
                value = value - 128
            change = value / 127.0
            self._value_norm += change
            if self._value_norm < 0:
                self._value_norm = 0
            if self._value_norm > 1:
                self._value_norm = 1
        self.update()

    def update(self):
        if self.curve == "exponential":
            value_scaled = scale_lin_exp(self._value_norm, 0, 1, self.range_min, self.range_max)
        elif self.curve == "linear":
            value_scaled = scale_lin_lin(self._value_norm, 0, 1, self.range_min, self.range_max)
        self.set_input("value", value_scaled)
