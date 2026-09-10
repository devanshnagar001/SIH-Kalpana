#ifndef WAKE_DETECTOR_H
#define WAKE_DETECTOR_H

#include <stdint.h>

// Per contract_v1.md
#define WAKE_CONFIDENCE_THRESHOLD 0.85f
#define WAKE_DEBOUNCE_MS          500

typedef struct {
    float confidence;
    uint32_t timestamp_ms;
} wake_event_t;

/**
 * FreeRTOS task: consumes confidence scores from detection_queue, applies
 * the threshold + debounce logic from contract_v1.md, and fires the wake
 * signal (GPIO pulse + log) when a genuine trigger is detected.
 */
void wake_detector_task(void *pvParameters);

#endif // WAKE_DETECTOR_H
