/**
 * audio_capture.c
 * I2S microphone capture for the ESP32-S3.
 *
 * Placeholder implementation: fills frames from the I2S driver where
 * available, otherwise synthesizes silence so the rest of the pipeline
 * (MFCC -> inference -> detection) has a real data stream to run against
 * during bring-up. Swap synthesize_silence() for the real I2S read once
 * the mic is wired up on the shared dev board.
 */

#include <string.h>
#include "audio_capture.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

extern QueueHandle_t audio_queue;

static const char *TAG = "audio_capture";

int audio_capture_init(void)
{
    // TODO: configure I2S peripheral per contract_v1.md
    //   - sample_rate = 16000
    //   - bits_per_sample = 16
    //   - channel = mono
    ESP_LOGI(TAG, "Audio capture initialized (stub)");
    return 0;
}

static void synthesize_silence(audio_frame_t *frame)
{
    memset(frame->samples, 0, sizeof(frame->samples));
}

void audio_capture_task(void *pvParameters)
{
    audio_capture_init();

    audio_frame_t frame;
    uint32_t t_ms = 0;

    for (;;) {
        // TODO: replace with i2s_channel_read() into frame.samples
        synthesize_silence(&frame);
        frame.timestamp_ms = t_ms;
        t_ms += 32; // ~512 samples @ 16kHz

        if (xQueueSend(audio_queue, &frame, pdMS_TO_TICKS(50)) != pdTRUE) {
            ESP_LOGW(TAG, "audio_queue full, dropping frame");
        }

        vTaskDelay(pdMS_TO_TICKS(32));
    }
}
