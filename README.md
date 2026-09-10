# SIH-Kalpana

**Low Latency and Efficient Voice Activator for Edge Devices**
Smart India Hackathon 2026 — Problem Statement **SIH26172** (ISRO, Hardware category)

## What this is

A fully on-device wake-word detector — trigger word **"Kalpana"** — running entirely
on an ESP32-S3. No cloud, no network round-trip: audio is captured, converted to MFCC
features, and passed through a lightweight neural keyword-spotting (KWS) model, all
in fixed-point C on-chip.

```
Mic  →  Framing/Windowing  →  MFCC  →  KWS Model  →  Wake Trigger
```

## Repo layout

| Folder | Contents |
|---|---|
| `contract/` | Frozen interface spec ("Contract v1") every module is built against — sample rate, MFCC params, tensor shapes, streaming format |
| `firmware/` | ESP32-S3 / FreeRTOS firmware — audio capture, inference, wake-word decision logic, output/network signaling |
| `mfcc/` | MFCC feature extraction — Python reference implementation and its fixed-point C port |
| `model/` | KWS model training pipeline and exported inference artifacts |
| `tests/` | Parity and sanity tests across audio, MFCC, and network modules |
| `data/` | Dataset notes and layout (raw audio not checked in — see `data/README.md`) |
| `docs/` | Architecture notes and diagrams |

## Status

Work in progress for SIH 2026 submission. See `contract/contract_v1.md` for the
locked interface all modules currently target, and `docs/ARCHITECTURE.md` for the
full pipeline breakdown.

## Team

Team ID: _TBD_
Team Name: _TBD_
