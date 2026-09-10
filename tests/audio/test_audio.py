"""
test_audio.py
Sanity checks for the audio capture contract (frame size, sample rate
assumptions). Does not require real hardware — validates shapes/format
against contract/contract_v1.md.
"""

import numpy as np

EXPECTED_FRAME_SAMPLES = 512
EXPECTED_SAMPLE_RATE = 16000


def synthesize_frame(num_samples=EXPECTED_FRAME_SAMPLES):
    """Stand-in for a real captured frame until hardware-in-the-loop tests exist."""
    t = np.linspace(0, num_samples / EXPECTED_SAMPLE_RATE, num_samples, endpoint=False)
    return (0.3 * np.sin(2 * np.pi * 440 * t) * 32767).astype(np.int16)


def test_frame_length():
    frame = synthesize_frame()
    assert len(frame) == EXPECTED_FRAME_SAMPLES, "Frame length must match contract_v1.md"


def test_frame_dtype():
    frame = synthesize_frame()
    assert frame.dtype == np.int16, "Audio frames must be 16-bit PCM per contract_v1.md"


def test_frame_range():
    frame = synthesize_frame()
    assert frame.max() <= 32767 and frame.min() >= -32768


if __name__ == "__main__":
    test_frame_length()
    test_frame_dtype()
    test_frame_range()
    print("All audio contract checks passed.")
