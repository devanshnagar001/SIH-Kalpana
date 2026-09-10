#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include <stdint.h>
#include "freertos/FreeRTOS.h"

// Must match contract_v1.md: 512 samples per frame @ 16kHz, 16-bit mono PCM
#define AUDIO_FRAME_SAMPLES 512

typedef struct {
    int16_t samples[AUDIO_FRAME_SAMPLES];
    uint32_t timestamp_ms;
} audio_frame_t;

/**
 * FreeRTOS task: reads from the I2S mic peripheral, packages fixed-size
 * frames, and pushes them onto the global audio_queue for the MFCC/inference
 * stage to consume.
 */
void audio_capture_task(void *pvParameters);

/** One-time I2S peripheral setup. Returns 0 on success. */
int audio_capture_init(void);

#endif // AUDIO_CAPTURE_H
