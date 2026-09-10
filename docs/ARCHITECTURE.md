# Architecture — Kalpana Voice Activator

## Pipeline Overview

```
 ┌──────────┐    ┌──────────────┐    ┌────────────┐    ┌──────────────┐    ┌────────────┐
 │   Mic    │ →  │ Audio Capture│ →  │    MFCC    │ →  │  KWS Model   │ →  │  Wake       │
 │ (I2S in) │    │  (FreeRTOS   │    │ (fixed-pt  │    │ (TFLite      │    │  Detector   │
 │          │    │   task)      │    │  C, per    │    │  Micro,      │    │ (threshold +│
 │          │    │              │    │  contract) │    │  int8)       │    │  debounce)  │
 └──────────┘    └──────────────┘    └────────────┘    └──────────────┘    └──────┬─────┘
                                                                                    │
                                                                                    ▼
                                                                          GPIO pulse + log
                                                                        (+ optional network
                                                                          status reporting)
```

## Task Breakdown (FreeRTOS)

| Task | Core | Priority | Role |
|---|---|---|---|
| `audio_capture_task` | 0 | 5 | Reads I2S mic, pushes fixed `audio_frame_t` frames to `audio_queue` |
| `inference_task` | 1 | 5 | Consumes MFCC frames, accumulates a sliding window, runs the TFLite Micro model |
| `wake_detector_task` | 1 | 5 | Applies confidence threshold + debounce, fires the wake signal |
| `network_task` | 0 | 4 | Heartbeat/status reporting, off the critical detection path |

## Why on-device (not cloud)

The problem statement (SIH26172) calls for **low latency** and **efficiency**
on edge devices. Routing audio to a cloud STT/wake-word service adds a
network round-trip (typically 200ms-1s+) and a hard dependency on
connectivity — unacceptable for the target deployment contexts (remote,
low-connectivity, or security-sensitive environments). Running the full
pipeline on an ESP32-S3 keeps detection local, private, and near-instant.

## Key Risk: Python/C MFCC Parity

The MFCC stage is implemented twice — once in Python (`mfcc/python/mfcc.py`,
the reference/training-time implementation) and once in fixed-point C
(`mfcc/c/mfcc.c`, the on-device implementation). These must produce
matching features, or the model (trained on the Python reference's output)
will see unfamiliar input at inference time and misbehave.

**Fallback plan:** if fixed-point parity isn't reached by the internal
deadline, swap in TFLite Micro's built-in `micro_frontend` feature
extractor in place of the custom C port, at the cost of losing some
fine-grained control over the feature extraction tuning.

## Module Ownership

See `contract/contract_v1.md` for the frozen interface every module below
is built against, and the project's team gate schedule (G1/G2/G3) for
integration checkpoints.
