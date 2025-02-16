#!/usr/bin/env python3

# --------------------------------------------------------------------------------
# SignalFlow command-line utility
# --------------------------------------------------------------------------------

from signalflow import *
import signalflow_examples
import subprocess
import argparse
import os


def run_configure():
    config_directory = os.path.expanduser("~/.signalflow")
    config_path = os.path.join(config_directory, "config")
    os.makedirs(config_directory, exist_ok=True)
    if not os.path.exists(config_path):
        with open(config_path, "w") as fd:
            fd.write("# This file contains configuration for SignalFlow synthesis engine.\n")
            fd.write("# Uncomment any configuration options you wish to use.\n")
            fd.write("# String values can be quoted in double-quotes.\n")
            fd.write("\n")
            fd.write("[audio]\n")
            fd.write("# output_device_name =\n")
            fd.write("# output_buffer_size =\n")
            fd.write("# cpu_usage_limit =\n")
    editor = os.getenv("EDITOR")
    if not editor:
        editor = "pico"
    subprocess.call([editor, config_path])


def run_test(frequency: float = 440,
             gain: float = 0.0,
             channels: int = 1,
             backend_name: str = None,
             output_device_name: str = None):
    config = AudioGraphConfig()
    if backend_name:
        config.backend_name = backend_name
    if output_device_name:
        config.output_device_name = output_device_name
    graph = AudioGraph(config=config)
    tone = SineOscillator(frequency)
    amplitude = db_to_amplitude(gain)
    attenuated = tone * amplitude
    channel_index = Counter(Impulse(1), 0, channels)
    panner = ChannelPanner(channels, attenuated, channel_index)
    panner.play()
    print("Generating test tone at %dHz. Press Ctrl-C to stop." % frequency)
    try:
        graph.wait()
    except KeyboardInterrupt:
        print()
        print("Exiting...")


def run_version():
    import signalflow
    print(signalflow.__version__)


def run_list_output_device_names(backend_name: str = None):
    output_device_names = AudioGraph.get_output_device_names(backend_name)
    print("Available output device names:")
    for name in output_device_names:
        print(" - %s" % name)


def run_list_input_device_names(backend_name: str = None):
    input_device_names = AudioGraph.get_input_device_names(backend_name)
    print("Available input device names:")
    for name in input_device_names:
        print(" - %s" % name)


def run_list_backend_names():
    backend_names = AudioGraph.get_backend_names()
    print("Available output backend names:")
    for name in backend_names:
        print(" - %s" % name)


def run_list_midi_output_device_names():
    try:
        import mido
    except ModuleNotFoundError:
        raise ModuleNotFoundError("The mido module is required for MIDI support. To install, run: pip3 install mido")

    output_names = mido.get_output_names()
    print("Available MIDI output device names:")
    for name in output_names:
        print(" - %s" % name)


def run_list_midi_input_device_names():
    try:
        import mido
    except ModuleNotFoundError:
        raise ModuleNotFoundError("The mido module is required for MIDI support. To install, run: pip3 install mido")

    input_names = mido.get_input_names()
    print("Available MIDI input device names:")
    for name in input_names:
        print(" - %s" % name)


def run_download_examples(output_directory: str):
    signalflow_examples.download_examples(output_directory)


def main():
    parser = argparse.ArgumentParser(description='SignalFlow command-line utility')
    subparsers = parser.add_subparsers(dest='command', required=True)

    # --------------------------------------------------------------------------------
    # Command: configure
    # --------------------------------------------------------------------------------
    configure = subparsers.add_parser('configure', aliases=['config'], help='edit signalflow configuration')

    # --------------------------------------------------------------------------------
    # Command: version
    # --------------------------------------------------------------------------------
    version = subparsers.add_parser('version', help='show signalflow version')

    # --------------------------------------------------------------------------------
    # Command: test
    # --------------------------------------------------------------------------------
    test = subparsers.add_parser('test', help='play a test tone')
    test.add_argument('--gain', type=float, help='tone level, in dB (default: -12)', default=-12)
    test.add_argument('--channels', '-c', type=int, help='number of channels to output, in sequence', default=1)
    test.add_argument('--frequency', type=float, help='tone frequency, in Hz (default: 440)', default=440)
    test.add_argument('--backend-name', type=str,
                      help='name of audio backend to use (default: system default backend)', default=None)
    test.add_argument('--output-device-name', type=str,
                      help='name of output device to use (default: system default output)', default=None)

    # --------------------------------------------------------------------------------
    # Command: list-output-device-names
    # --------------------------------------------------------------------------------
    list_output_device_names = subparsers.add_parser('list-output-device-names', help='list available output devices')
    list_output_device_names.add_argument('--backend-name', type=str,
                                          help='name of audio backend to use (default: system default backend)',
                                          default=None)

    # --------------------------------------------------------------------------------
    # Command: list-input-device-names
    # --------------------------------------------------------------------------------
    list_input_device_names = subparsers.add_parser('list-input-device-names', help='list available input devices')
    list_input_device_names.add_argument('--backend-name', type=str,
                                         help='name of audio backend to use (default: system default backend)',
                                         default=None)

    # --------------------------------------------------------------------------------
    # Command: list-output-backend-names
    # --------------------------------------------------------------------------------
    list_backend_names = subparsers.add_parser('list-backend-names',
                                               help='list available output backends')
    help = subparsers.add_parser('help', help='show help')

    # --------------------------------------------------------------------------------
    # Command: list-midi-output-device-names
    # --------------------------------------------------------------------------------
    list_midi_output_device_names = subparsers.add_parser('list-midi-output-device-names',
                                                          help='list available MIDI output devices')

    # --------------------------------------------------------------------------------
    # Command: list-midi-input-device-names
    # --------------------------------------------------------------------------------
    list_midi_input_device_names = subparsers.add_parser('list-midi-input-device-names',
                                                         help='list available MIDI input devices')

    # --------------------------------------------------------------------------------
    # Command: download-examples
    # --------------------------------------------------------------------------------
    download_examples = subparsers.add_parser('download-examples', help='download example scripts')
    download_examples.add_argument('--output-directory', '-o', type=str,
                                   help='path to output directory (default: examples)',
                                   default='examples')

    args = parser.parse_args()
    if args.command == 'configure' or args.command == 'config':
        run_configure()
    elif args.command == 'version':
        run_version()
    elif args.command == 'test':
        run_test(args.frequency, args.gain, args.channels, args.backend_name, args.output_device_name)
    elif args.command == 'list-output-device-names':
        run_list_output_device_names(args.backend_name)
    elif args.command == 'list-input-device-names':
        run_list_input_device_names(args.backend_name)
    elif args.command == 'list-backend-names':
        run_list_backend_names()
    elif args.command == 'list-midi-output-device-names':
        run_list_midi_output_device_names()
    elif args.command == 'list-midi-input-device-names':
        run_list_midi_input_device_names()
    elif args.command == 'download-examples':
        run_download_examples(args.output_directory)
    elif args.command == 'help':
        parser.print_help()


if __name__ == '__main__':
    main()
