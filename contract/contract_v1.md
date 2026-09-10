# Contract v1 — Locked Interface Spec

This document is the single source of truth for every module boundary in the
pipeline. All firmware, MFCC, and model code must conform to these values.
Any change requires team sign-off and a version bump (`contract_v2.md`, etc.).

## Audio Input

| Parameter | Value |
|---|---|
| Sample rate | 16,000 Hz |
| Bit depth | 16-bit signed PCM |
| Channels | 1 (mono) |
| Frame size (capture buffer) | 512 samples |

## MFCC Parameters

| Parameter | Value |
|---|---|
| Window length | 25 ms (400 samples @ 16kHz) |
| Hop length | 10 ms (160 samples @ 16kHz) |
| Window function | Hamming |
| FFT size | 512 |
| Number of Mel filterbanks | 40 |
| Number of MFCC coefficients (output) | 13 |
| Pre-emphasis coefficient | 0.97 |

## Model Input Tensor

| Parameter | Value |
|---|---|
| Shape | `[49, 13, 1]` (time steps × MFCC coeffs × channel) |
| Dtype | `int8` (quantized) |
| Normalization | per-feature mean/scale baked into quantization params |

## Streaming Format (audio task → MFCC task → inference task)

Each stage communicates over a FreeRTOS queue carrying a fixed-size struct:

```c
typedef struct {
    int16_t samples[512];   // raw PCM frame
    uint32_t timestamp_ms;
} audio_frame_t;

typedef struct {
    int8_t  mfcc[13];       // one MFCC frame (13 coeffs)
    uint32_t timestamp_ms;
} mfcc_frame_t;
```

## Wake Decision Output

| Parameter | Value |
|---|---|
| Trigger threshold (model confidence) | 0.85 |
| Debounce window | 500 ms (ignore repeat triggers within this window) |
| Output signal | GPIO pulse (10 ms high) + log line over UART |

## Change Log

- **v1** — initial freeze, team gate G1
