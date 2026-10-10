import signalflow
import pytest
import math

def test_clip():
    assert signalflow.clip(-0.5, 0, 1) == 0.0
    assert signalflow.clip(0.0, 0, 1) == 0.0
    assert signalflow.clip(0.5, 0, 1) == 0.5
    assert signalflow.clip(1.0, 0, 1) == 1.0
    assert signalflow.clip(1.5, 0, 1) == 1.0

def test_wrap():
    assert signalflow.wrap(0.0, 0, 1) == 0.0
    assert signalflow.wrap(0.25, 0, 1) == 0.25
    assert signalflow.wrap(1.0, 0, 1) == 0.0
    assert signalflow.wrap(1.25, 0, 1) == pytest.approx(0.25)
    assert signalflow.wrap(2.5, 0, 1) == pytest.approx(0.5)
    assert signalflow.wrap(7, 2, 5) == pytest.approx(4)

def test_wrap_below_min():
    assert signalflow.wrap(-0.25, 0, 1) == pytest.approx(0.75)
    assert signalflow.wrap(-1.25, 0, 1) == pytest.approx(0.75)
    assert signalflow.wrap(1, 2, 5) == pytest.approx(4)

def test_fold():
    assert signalflow.fold(0.0, 0, 1) == 0.0
    assert signalflow.fold(0.25, 0, 1) == 0.25
    assert signalflow.fold(1.0, 0, 1) == 1.0
    assert signalflow.fold(1.25, 0, 1) == pytest.approx(0.75)
    assert signalflow.fold(2.5, 0, 1) == pytest.approx(0.5)
    assert signalflow.fold(6, 2, 5) == pytest.approx(4)

def test_fold_below_min():
    assert signalflow.fold(-0.25, 0, 1) == pytest.approx(0.25)
    assert signalflow.fold(-1.25, 0, 1) == pytest.approx(0.75)
    assert signalflow.fold(1, 2, 5) == pytest.approx(3)

def test_scale_lin_lin():
    assert signalflow.scale_lin_lin(0, 0, 10, 100, 200) == 100
    assert signalflow.scale_lin_lin(5, 0, 10, 100, 200) == 150
    assert signalflow.scale_lin_lin(10, 0, 10, 100, 200) == 200

    # Inverted output range
    assert signalflow.scale_lin_lin(2.5, 0, 10, 1, 0) == pytest.approx(0.75)

    # Values outside the input range are extrapolated, not clipped
    assert signalflow.scale_lin_lin(-5, 0, 10, 100, 200) == 50
    assert signalflow.scale_lin_lin(20, 0, 10, 100, 200) == 300

def test_scale_lin_exp():
    assert signalflow.scale_lin_exp(0, 0, 1, 20, 20000) == pytest.approx(20)
    assert signalflow.scale_lin_exp(1, 0, 1, 20, 20000) == pytest.approx(20000)

    # Midpoint of the linear range maps to the geometric mean of the output range
    assert signalflow.scale_lin_exp(0.5, 0, 1, 20, 20000) == pytest.approx(math.sqrt(20 * 20000))
    assert signalflow.scale_lin_exp(1 / 3, 0, 1, 10, 10000) == pytest.approx(100)

    # Values outside the input range are clipped
    assert signalflow.scale_lin_exp(-1, 0, 1, 20, 20000) == 20
    assert signalflow.scale_lin_exp(2, 0, 1, 20, 20000) == 20000

def test_scale_exp_lin():
    assert signalflow.scale_exp_lin(20, 20, 20000, 0, 1) == pytest.approx(0)
    assert signalflow.scale_exp_lin(20000, 20, 20000, 0, 1) == pytest.approx(1)

    # Geometric mean of the input range maps to the midpoint of the output range
    assert signalflow.scale_exp_lin(math.sqrt(20 * 20000), 20, 20000, 0, 1) == pytest.approx(0.5)
    assert signalflow.scale_exp_lin(100, 10, 10000, 0, 3) == pytest.approx(1)

    # Inverse of scale_lin_exp
    for value in [0.1, 0.25, 0.9]:
        scaled = signalflow.scale_lin_exp(value, 0, 1, 20, 20000)
        assert signalflow.scale_exp_lin(scaled, 20, 20000, 0, 1) == pytest.approx(value)

    # Values outside the input range are clipped
    assert signalflow.scale_exp_lin(10, 20, 20000, 0, 1) == 0
    assert signalflow.scale_exp_lin(40000, 20, 20000, 0, 1) == 1

def test_frequency_to_midi_note():
    assert signalflow.frequency_to_midi_note(440) == pytest.approx(69)
    assert signalflow.frequency_to_midi_note(880) == pytest.approx(81)
    assert signalflow.frequency_to_midi_note(220) == pytest.approx(57)
    assert signalflow.frequency_to_midi_note(261.6256) == pytest.approx(60, abs=1e-4)

    # Non-integer note values for frequencies between semitones
    assert signalflow.frequency_to_midi_note(440 * 2 ** (0.5 / 12)) == pytest.approx(69.5, abs=1e-4)

def test_midi_note_to_frequency():
    assert signalflow.midi_note_to_frequency(69) == pytest.approx(440)
    assert signalflow.midi_note_to_frequency(81) == pytest.approx(880)
    assert signalflow.midi_note_to_frequency(57) == pytest.approx(220)
    assert signalflow.midi_note_to_frequency(60) == pytest.approx(261.6256, abs=1e-3)
    assert signalflow.midi_note_to_frequency(69.5) == pytest.approx(440 * 2 ** (0.5 / 12), rel=1e-5)

    # Inverse of frequency_to_midi_note
    for note in [21, 48.25, 108]:
        frequency = signalflow.midi_note_to_frequency(note)
        assert signalflow.frequency_to_midi_note(frequency) == pytest.approx(note, abs=1e-4)

def test_db_to_amplitude():
    assert signalflow.db_to_amplitude(0.0) == 1.0
    assert signalflow.db_to_amplitude(-12) == pytest.approx(10 ** (-12 / 20), rel=0.00001)
    assert signalflow.db_to_amplitude(24) == pytest.approx(10 ** (24 / 20), rel=0.00001)

def test_amplitude_to_db():
    assert signalflow.amplitude_to_db(1.0) == 0.0
    assert signalflow.amplitude_to_db(0.5) == pytest.approx(20.0 * math.log10(0.5))
    assert signalflow.amplitude_to_db(0.01) == pytest.approx(20.0 * math.log10(0.01))

def test_calculate_decay_coefficient():
    decay_time = 0.5
    sample_rate = 48000
    decay_level = 0.001

    exponent = signalflow.calculate_decay_coefficient(decay_time, sample_rate, decay_level)
    rv = exponent ** (decay_time * sample_rate)
    assert rv == pytest.approx(decay_level, abs=1e-6)

def test_random_seed():
    signalflow.random_seed(123)
    a = [signalflow.random_uniform() for _ in range(10)]
    signalflow.random_seed(123)
    b = [signalflow.random_uniform() for _ in range(10)]
    signalflow.random_seed(456)
    c = [signalflow.random_uniform() for _ in range(10)]
    assert a == b
    assert a != c

def test_random_uniform():
    signalflow.random_seed(123)
    values = [signalflow.random_uniform() for _ in range(10000)]
    assert all(0 <= value < 1 for value in values)
    assert sum(values) / len(values) == pytest.approx(0.5, abs=0.02)

    values = [signalflow.random_uniform(-10, 30) for _ in range(10000)]
    assert all(-10 <= value < 30 for value in values)
    assert sum(values) / len(values) == pytest.approx(10, abs=1)

def test_random_integer():
    signalflow.random_seed(123)
    values = [signalflow.random_integer(0, 4) for _ in range(10000)]
    assert set(values) == {0, 1, 2, 3}
    for n in range(4):
        assert values.count(n) / len(values) == pytest.approx(0.25, abs=0.02)

    values = [signalflow.random_integer(10, 20) for _ in range(10000)]
    assert set(values) == set(range(10, 20))

def test_random_integer_unbiased():
    signalflow.random_seed(123)
    values = [signalflow.random_integer(5, 8) for _ in range(30000)]
    assert set(values) == {5, 6, 7}
    for n in range(5, 8):
        assert values.count(n) / len(values) == pytest.approx(1 / 3, abs=0.02)

def test_random_integer_negative():
    signalflow.random_seed(123)
    values = [signalflow.random_integer(-3, 3) for _ in range(1000)]
    assert set(values) == set(range(-3, 3))

def test_random_exponential():
    signalflow.random_seed(123)
    values = [signalflow.random_exponential(20, 20000) for _ in range(10000)]
    assert all(20 <= value <= 20000 for value in values)

    # Half of the values should fall below the geometric mean of the range
    geometric_mean = math.sqrt(20 * 20000)
    proportion_below = sum(value < geometric_mean for value in values) / len(values)
    assert proportion_below == pytest.approx(0.5, abs=0.02)
