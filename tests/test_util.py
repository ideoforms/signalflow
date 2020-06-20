from libsignal import signal_clip
import pytest

def test_clip():
    assert signal_clip(-0.5, 0, 1) == 0.0
    assert signal_clip(0.0, 0, 1) == 0.0
    assert signal_clip(0.5, 0, 1) == 0.5
    assert signal_clip(1.0, 0, 1) == 1.0
    assert signal_clip(1.5, 0, 1) == 1.0

@pytest.mark.skip
def test_scale():
    pass

@pytest.mark.skip
def test_scale_linexp():
    pass

@pytest.mark.skip
def test_scale_explin():
    pass

@pytest.mark.skip
def test_frequency_to_midi_note():
    pass

@pytest.mark.skip
def test_midi_note_to_frequency():
    pass

@pytest.mark.skip
def test_db_to_amp():
    pass

@pytest.mark.skip
def test_amp_to_db():
    pass

@pytest.mark.skip
def test_save_block_to_text_file():
    pass

@pytest.mark.skip
def test_save_block_to_wav_file():
    pass