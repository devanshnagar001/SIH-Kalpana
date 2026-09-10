#ifndef MFCC_H
#define MFCC_H

#include <stdint.h>

// Must match contract/contract_v1.md and mfcc/python/mfcc.py
#define MFCC_SAMPLE_RATE      16000
#define MFCC_FRAME_LEN        400   // 25ms
#define MFCC_FRAME_HOP        160   // 10ms
#define MFCC_FFT_SIZE         512
#define MFCC_NUM_MEL_FILTERS  40
#define MFCC_NUM_COEFFS       13
#define MFCC_PRE_EMPHASIS_Q15 31785  // 0.97 in Q15 fixed point

/**
 * Fixed-point MFCC extraction for a single frame of MFCC_FRAME_LEN
 * int16 PCM samples. Output is MFCC_NUM_COEFFS int8 coefficients,
 * matching the model's quantized input tensor (contract_v1.md).
 *
 * This is a submission-stage scaffold: the pipeline stages are laid out
 * (pre-emphasis -> windowing -> FFT -> mel filterbank -> log -> DCT) but
 * FFT/DCT bodies are stubbed pending parity validation against
 * mfcc/python/mfcc.py.
 */
void mfcc_init(void);

void mfcc_compute_frame(const int16_t *pcm_frame, int8_t out_coeffs[MFCC_NUM_COEFFS]);

#endif // MFCC_H
