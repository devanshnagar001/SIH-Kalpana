#ifndef INFERENCE_H
#define INFERENCE_H

#include <stdint.h>

// Model input tensor per contract_v1.md: [49, 13, 1] int8
#define MODEL_INPUT_TIMESTEPS 49
#define MODEL_INPUT_MFCC      13

typedef struct {
    int8_t mfcc[MODEL_INPUT_MFCC];
    uint32_t timestamp_ms;
} mfcc_frame_t;

/**
 * FreeRTOS task: consumes mfcc_frame_t from mfcc_queue, accumulates a
 * MODEL_INPUT_TIMESTEPS-length window, runs the TFLite Micro interpreter,
 * and pushes a confidence score to the wake detector.
 */
void inference_task(void *pvParameters);

/** Loads the TFLite Micro model and sets up the interpreter/arena. */
int inference_init(void);

/** Runs one forward pass; returns wake-word confidence in [0.0, 1.0]. */
float inference_run(const int8_t mfcc_window[MODEL_INPUT_TIMESTEPS][MODEL_INPUT_MFCC]);

#endif // INFERENCE_H
