"""
mfcc.py
Reference MFCC feature extractor — floating point, NumPy-based.
This is the "ground truth" the fixed-point C port (mfcc/c/mfcc.c) is
validated against. Parameters here must match contract/contract_v1.md.
"""

import numpy as np

SAMPLE_RATE = 16000
FRAME_LEN = 400          # 25ms @ 16kHz
FRAME_HOP = 160           # 10ms @ 16kHz
FFT_SIZE = 512
NUM_MEL_FILTERS = 40
NUM_MFCC = 13
PRE_EMPHASIS = 0.97


def pre_emphasize(signal: np.ndarray, coeff: float = PRE_EMPHASIS) -> np.ndarray:
    return np.append(signal[0], signal[1:] - coeff * signal[:-1])


def frame_signal(signal: np.ndarray, frame_len: int = FRAME_LEN, hop: int = FRAME_HOP) -> np.ndarray:
    num_frames = 1 + max(0, (len(signal) - frame_len) // hop)
    frames = np.zeros((num_frames, frame_len))
    for i in range(num_frames):
        start = i * hop
        frames[i] = signal[start:start + frame_len]
    return frames


def hamming_window(frames: np.ndarray) -> np.ndarray:
    return frames * np.hamming(frames.shape[1])


def power_spectrum(frames: np.ndarray, fft_size: int = FFT_SIZE) -> np.ndarray:
    mag = np.abs(np.fft.rfft(frames, n=fft_size))
    return (1.0 / fft_size) * (mag ** 2)


def mel_filterbank(num_filters: int = NUM_MEL_FILTERS,
                    fft_size: int = FFT_SIZE,
                    sample_rate: int = SAMPLE_RATE) -> np.ndarray:
    def hz_to_mel(hz):
        return 2595 * np.log10(1 + hz / 700)

    def mel_to_hz(mel):
        return 700 * (10 ** (mel / 2595) - 1)

    low_mel, high_mel = 0, hz_to_mel(sample_rate / 2)
    mel_points = np.linspace(low_mel, high_mel, num_filters + 2)
    hz_points = mel_to_hz(mel_points)
    bins = np.floor((fft_size + 1) * hz_points / sample_rate).astype(int)

    fbank = np.zeros((num_filters, fft_size // 2 + 1))
    for m in range(1, num_filters + 1):
        f_left, f_center, f_right = bins[m - 1], bins[m], bins[m + 1]
        for k in range(f_left, f_center):
            fbank[m - 1, k] = (k - f_left) / max(1, (f_center - f_left))
        for k in range(f_center, f_right):
            fbank[m - 1, k] = (f_right - k) / max(1, (f_right - f_center))
    return fbank


def dct(mel_energies: np.ndarray, num_mfcc: int = NUM_MFCC) -> np.ndarray:
    from scipy.fftpack import dct as scipy_dct
    return scipy_dct(mel_energies, type=2, axis=1, norm="ortho")[:, :num_mfcc]


def compute_mfcc(signal: np.ndarray, sample_rate: int = SAMPLE_RATE) -> np.ndarray:
    """
    signal: 1D int16 or float PCM array
    returns: [num_frames, NUM_MFCC] float array
    """
    signal = signal.astype(np.float64)
    emphasized = pre_emphasize(signal)
    frames = hamming_window(frame_signal(emphasized))
    pspec = power_spectrum(frames)

    fbank = mel_filterbank(sample_rate=sample_rate)
    mel_energies = np.dot(pspec, fbank.T)
    mel_energies = np.where(mel_energies == 0, np.finfo(float).eps, mel_energies)
    log_mel = np.log(mel_energies)

    return dct(log_mel)


if __name__ == "__main__":
    # Quick smoke test with synthetic audio
    duration_s = 1.0
    t = np.linspace(0, duration_s, int(SAMPLE_RATE * duration_s), endpoint=False)
    test_signal = (0.5 * np.sin(2 * np.pi * 440 * t) * 32767).astype(np.int16)

    features = compute_mfcc(test_signal)
    print(f"Input samples: {len(test_signal)}")
    print(f"Output MFCC shape: {features.shape}  (frames, coeffs)")
    print("First frame:", features[0])
