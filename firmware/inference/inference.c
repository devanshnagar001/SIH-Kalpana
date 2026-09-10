/**
 * inference.c
 * TFLite Micro wrapper around the exported KWS model
 * (see model/exported/kalpana_kws.tflite).
 *
 * Placeholder implementation: builds the sliding window of MFCC frames
 * and calls inference_run(), which currently returns a stubbed score.
 * Wire in the real TFLite Micro interpreter (tflite::MicroInterpreter)
 * once model/exported/kalpana_kws.tflite is committed.
 */

#include <string.h>
#include "inference.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

extern QueueHandle_t mfcc_queue;
extern QueueHandle_t detection_queue;

typedef struct {
    float confidence;
    uint32_t timestamp_ms;
} wake_event_t;

static const char *TAG = "inference";

static int8_t window[MODEL_INPUT_TIMESTEPS][MODEL_INPUT_MFCC];
static int window_fill = 0;

int inference_init(void)
{
    // TODO: load model/exported/kalpana_kws.tflite, allocate tensor arena,
    //       build tflite::MicroInterpreter
    memset(window, 0, sizeof(window));
    window_fill = 0;
    ESP_LOGI(TAG, "Inference engine initialized (stub)");
    return 0;
}

float inference_run(const int8_t mfcc_window[MODEL_INPUT_TIMESTEPS][MODEL_INPUT_MFCC])
{
    // TODO: copy mfcc_window into interpreter->input(0), Invoke(), read output(0)
    // Stub: return a low, non-triggering confidence so downstream logic
    // (thresholding, debounce) can be exercised end-to-end.
    (void)mfcc_window;
    return 0.10f;
}

void inference_task(void *pvParameters)
{
    inference_init();
    mfcc_frame_t frame;

    for (;;) {
        if (xQueueReceive(mfcc_queue, &frame, portMAX_DELAY) == pdTRUE) {
            memcpy(window[window_fill % MODEL_INPUT_TIMESTEPS], frame.mfcc, MODEL_INPUT_MFCC);
            window_fill++;

            if (window_fill >= MODEL_INPUT_TIMESTEPS) {
                float confidence = inference_run(window);
                wake_event_t evt = { .confidence = confidence, .timestamp_ms = frame.timestamp_ms };
                xQueueSend(detection_queue, &evt, 0);
            }
        }
    }
}
