import urllib.request
import urllib.parse
import logging
import ssl
import os

# --------------------------------------------------------------------------------
# ignore local expired SSL certificates
# --------------------------------------------------------------------------------
ssl._create_default_https_context = ssl._create_unverified_context

logger = logging.getLogger(__name__)

def _download_files_from_url(url_prefix: str,
                             filenames: list[str],
                             output_directory: str) -> None:
    for filename in filenames:
        url = "%s/%s" % (url_prefix, urllib.parse.quote(filename))
        output_path = os.path.join(output_directory, filename)
        if os.path.exists(output_path):
            print(" - Not overwriting existing file: %s" % filename)
        else:
            urllib.request.urlretrieve(url, output_path)
            os.chmod(output_path, 0o755)
            print(" - Downloaded: %s" % filename)

def download_examples(output_directory: str = "examples") -> None:
    """
    Download all example scripts from GitHub to the current directory.
    """
    url_prefix = "https://raw.githubusercontent.com/ideoforms/signalflow/master/examples"
    examples = [
        "audio-through-example.py",
        "buffer-play-example.py",
        "chaotic-feedback-example.py",
        "euclidean-rhythm-example.py",
        "granulation-example.py",
        "hello-world-example.py",
        "karplus-strong-example.py",
        "list-output-devices.py",
        "midi-fm-voicer-example.py",
        "midi-keyboard-example.py",
        "modulation-example.py",
        "sequencing-example.py",
        "wavetable-2d-example.py",
    ]
    audio_files = [
        "stereo-count.wav"
    ]
    local_audio_dir = os.path.join(output_directory, "audio")
    os.makedirs(output_directory, exist_ok=True)
    os.makedirs(local_audio_dir, exist_ok=True)

    print("Downloading examples to: %s" % output_directory)
    _download_files_from_url(url_prefix, examples, output_directory)

    audio_output_directory = os.path.join(output_directory, "audio")
    audio_url_prefix = "%s/audio" % (url_prefix)
    print("Downloading example audio to: %s" % audio_output_directory)
    _download_files_from_url(audio_url_prefix, audio_files, audio_output_directory)


def download_notebooks(output_directory: str = "notebooks") -> None:
    """
    Download all example scripts from GitHub to the current directory.
    """
    url_prefix = "https://raw.githubusercontent.com/ideoforms/signalflow/master/examples/notebooks"
    notebooks = [
        "01. Hello World.ipynb",
        "02. Patch Example.ipynb",
    ]
    os.makedirs(output_directory, exist_ok=True)
    print("Downloading examples to: %s" % output_directory)
    _download_files_from_url(url_prefix, notebooks, output_directory)


if __name__ == "__main__":
    download_examples()
