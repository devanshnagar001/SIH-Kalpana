from python_speech_features import mfcc
import scipy.io.wavfile as wav
import numpy as np

rate, signal = wav.read("TestAudioWAV.wav")

target_len = 15840  # gives exactly 49 frames with 30ms window / 20ms hop
signal = signal[:target_len] if len(signal) >= target_len else np.pad(signal, (0, target_len - len(signal)))

print("Signal length:", len(signal))

features = mfcc(signal, samplerate=rate, winlen=0.03, winstep=0.02, numcep=10)
print(features.shape)  # should now print (49, 10)

np.savetxt("golden.txt", features)

"""def extract_mfcc(wav_path):
    rate, signal = wav.read(wav_path)
    target_len = 15840
    signal = signal[:target_len] if len(signal) >= target_len else np.pad(signal, (0, target_len - len(signal)))
    return mfcc(signal, samplerate=rate, winlen=0.03, winstep=0.02, numcep=10)"""
