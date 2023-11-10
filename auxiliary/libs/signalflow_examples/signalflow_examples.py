import urllib.request
import logging
import ssl

#--------------------------------------------------------------------------------
# ignore local expired SSL certificates
#--------------------------------------------------------------------------------
ssl._create_default_https_context = ssl._create_unverified_context

logger = logging.getLogger(__name__)

def download_examples() -> None:
    """
    Download all example scripts from GitHub to the current directory.
    """
    url_prefix = "https://raw.githubusercontent.com/ideoforms/signalflow/master/examples/"
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
    for filename in examples:
        url = "%s%s" % (url_prefix, filename)
        urllib.request.urlretrieve(url, filename)
        print(" - Downloaded: %s" % filename)

if __name__ == "__main__":
    download_examples()