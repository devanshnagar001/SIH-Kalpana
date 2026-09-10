# Data

Raw and processed audio for training/testing the "Kalpana" KWS model is
**not checked into this repo** (audio datasets are large and often carry
their own licensing/consent constraints for recorded voices).

## Expected layout (local, gitignored)

```
data/
├── kalpana/          # positive examples: recordings of the wake word "Kalpana"
│   ├── sample_001.wav
│   └── ...
├── background/        # negative examples: other speech, silence, ambient noise
│   ├── sample_001.wav
│   └── ...
└── features/           # precomputed MFCC .npy arrays (see mfcc/python/mfcc.py)
    ├── kalpana/
    └── background/
```

## Recording guidelines (draft)

- 16kHz, 16-bit mono WAV, matching `contract/contract_v1.md`
- Aim for a mix of speakers, distances from mic, and background noise levels
  for the positive ("kalpana") class
- Negative class should include general speech, silence, and common ambient
  sounds from the deployment environment

## Status

Dataset collection not yet started — this README defines the target
structure so `model/training/train.py` has a stable path to load from
once recordings are available.
