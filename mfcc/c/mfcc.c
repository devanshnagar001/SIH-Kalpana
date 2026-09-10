/**
 * mfcc.c
 * Fixed-point port of mfcc/python/mfcc.py, targeting the ESP32-S3.
 *
 * Submission-stage scaffold: pre-emphasis is implemented in Q15 fixed
 * point below. FFT, mel filterbank energy, log, and DCT stages are
 * structured but stubbed (TODO) pending the Python<->C parity pass
 * tracked in tests/mfcc/test_mfcc_parity.py.
 */

#include <string.h>
#include "mfcc.h"

static int16_t emphasized[MFCC_FRAME_LEN];
static int32_t power_spectrum[MFCC_FFT_SIZE / 2 + 1];
static int32_t mel_energies[MFCC_NUM_MEL_FILTERS];

void mfcc_init(void)
{
    memset(emphasized, 0, sizeof(emphasized));
    memset(power_spectrum, 0, sizeof(power_spectrum));
    memset(mel_energies, 0, sizeof(mel_energies));
}

/** Q15 fixed-point pre-emphasis: y[n] = x[n] - 0.97 * x[n-1] */
static void pre_emphasize(const int16_t *in, int16_t *out, int len)
{
    out[0] = in[0];
    for (int i = 1; i < len; i++) {
        int32_t prev_scaled = ((int32_t)in[i - 1] * MFCC_PRE_EMPHASIS_Q15) >> 15;
        out[i] = (int16_t)(in[i] - prev_scaled);
    }
}

/** TODO: fixed-point radix-2 FFT + magnitude^2, matching mfcc.py's power_spectrum() */
static void compute_power_spectrum(const int16_t *frame, int32_t *out)
{
    (void)frame;
    memset(out, 0, sizeof(int32_t) * (MFCC_FFT_SIZE / 2 + 1));
}

/** TODO: apply precomputed fixed-point mel filterbank to power_spectrum */
static void apply_mel_filterbank(const int32_t *pspec, int32_t *out_mel)
{
    (void)pspec;
    memset(out_mel, 0, sizeof(int32_t) * MFCC_NUM_MEL_FILTERS);
}

/** TODO: fixed-point log + DCT-II, quantized to int8 per contract_v1.md */
static void log_dct_quantize(const int32_t *mel, int8_t *out_coeffs)
{
    (void)mel;
    memset(out_coeffs, 0, MFCC_NUM_COEFFS);
}

void mfcc_compute_frame(const int16_t *pcm_frame, int8_t out_coeffs[MFCC_NUM_COEFFS])
{
    pre_emphasize(pcm_frame, emphasized, MFCC_FRAME_LEN);
    compute_power_spectrum(emphasized, power_spectrum);
    apply_mel_filterbank(power_spectrum, mel_energies);
    log_dct_quantize(mel_energies, out_coeffs);
}
