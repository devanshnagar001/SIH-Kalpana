/**
 * main.c
 * Entry point for the Kalpana voice-activator firmware.
 * Boots FreeRTOS tasks for: audio capture -> MFCC -> inference -> detection -> network.
 *
 * NOTE: This is a submission-stage scaffold. Task bodies contain placeholder
 * logic so the pipeline structure compiles and runs end-to-end; numerical
 * correctness of MFCC/model output is tracked separately (see mfcc/ and
 * model/) and is not required for this build to be considered complete.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "audio_capture.h"
#include "inference.h"
#include "wake_detector.h"
#include "network.h"

static const char *TAG = "kalpana_main";

QueueHandle_t audio_queue;
QueueHandle_t mfcc_queue;
QueueHandle_t detection_queue;

#define AUDIO_QUEUE_LEN      8
#define MFCC_QUEUE_LEN       8
#define DETECTION_QUEUE_LEN  4

void app_main(void)
{
    ESP_LOGI(TAG, "Kalpana voice activator booting...");

    audio_queue     = xQueueCreate(AUDIO_QUEUE_LEN, sizeof(audio_frame_t));
    mfcc_queue      = xQueueCreate(MFCC_QUEUE_LEN, sizeof(mfcc_frame_t));
    detection_queue = xQueueCreate(DETECTION_QUEUE_LEN, sizeof(wake_event_t));

    if (!audio_queue || !mfcc_queue || !detection_queue) {
        ESP_LOGE(TAG, "Queue creation failed, halting.");
        return;
    }

    xTaskCreatePinnedToCore(audio_capture_task, "audio_capture", 4096, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(inference_task,      "inference",     8192, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(wake_detector_task,  "wake_detector", 4096, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(network_task,        "network",       4096, NULL, 4, NULL, 0);

    ESP_LOGI(TAG, "All tasks started. Listening for wake word 'Kalpana'.");
}
