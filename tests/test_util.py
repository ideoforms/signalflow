import signalflow
import pytest
import math

def test_clip():
    assert signalflow.clip(-0.5, 0, 1) == 0.0
    assert signalflow.clip(0.0, 0, 1) == 0.0
    assert signalflow.clip(0.5, 0, 1) == 0.5
    assert signalflow.clip(1.0, 0, 1) == 1.0
    assert signalflow.clip(1.5, 0, 1) == 1.0

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

def test_db_to_amplitude():
    assert signalflow.db_to_amplitude(0.0) == 1.0
    assert signalflow.db_to_amplitude(-12) == pytest.approx(10 ** (-12 / 20), rel=0.00001)
    assert signalflow.db_to_amplitude(24) == pytest.approx(10 ** (24 / 20), rel=0.00001)

def test_amplitude_to_db():
    assert signalflow.amplitude_to_db(1.0) == 0.0
    assert signalflow.amplitude_to_db(0.5) == pytest.approx(20.0 * math.log10(0.5))
    assert signalflow.amplitude_to_db(0.01) == pytest.approx(20.0 * math.log10(0.01))

@pytest.mark.skip
def test_save_block_to_text_file():
    pass

@pytest.mark.skip
def test_save_block_to_wav_file():
    pass