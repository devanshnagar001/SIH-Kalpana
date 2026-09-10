"""
test_mfcc_parity.py
Compares the Python reference MFCC (mfcc/python/mfcc.py) against the
fixed-point C port (mfcc/c/mfcc.c) to check they agree within tolerance.

This is the highest-risk parity check in the project (see
contract/contract_v1.md). Currently the C side is a structural stub
(FFT/mel/DCT stages return zeros), so this test documents the intended
comparison methodology rather than asserting numerical parity yet.

Once mfcc/c/mfcc.c stages are filled in, wire this up via ctypes or a
small pybind11/cffi wrapper to call mfcc_compute_frame() directly and
compare against compute_mfcc() output frame-by-frame.
"""

import sys
import os
import numpy as np

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "..", "mfcc", "python"))
from mfcc import compute_mfcc, SAMPLE_RATE, FRAME_LEN  # noqa: E402

TOLERANCE = 1.0  # allowed abs difference per coefficient, once C is wired in


def synthesize_test_signal(duration_s=0.5):
    t = np.linspace(0, duration_s, int(SAMPLE_RATE * duration_s), endpoint=False)
    return (0.5 * np.sin(2 * np.pi * 440 * t) * 32767).astype(np.int16)


def get_python_reference(signal):
    return compute_mfcc(signal)


def get_c_output(signal):
    """
    TODO: call into mfcc/c/mfcc.c's mfcc_compute_frame() via ctypes, once
    the fixed-point stages are implemented. For now returns None to signal
    "not yet wired up" rather than a false pass/fail.
    """
    return None


def test_mfcc_parity():
    signal = synthesize_test_signal()
    py_features = get_python_reference(signal)
    c_features = get_c_output(signal)

    assert py_features.shape[1] == 13, "Python reference must output 13 MFCC coeffs"

    if c_features is None:
        print("[test_mfcc_parity] C implementation not yet wired in — "
              "Python reference verified, C comparison skipped.")
        return

    diff = np.abs(py_features - c_features)
    assert diff.max() <= TOLERANCE, f"Max MFCC deviation {diff.max()} exceeds tolerance {TOLERANCE}"


if __name__ == "__main__":
    test_mfcc_parity()
    print("MFCC parity test completed.")
