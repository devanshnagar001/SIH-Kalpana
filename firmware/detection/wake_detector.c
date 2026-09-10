/**
 * wake_detector.c
 * Applies threshold + debounce logic to model confidence scores and fires
 * the wake signal (GPIO pulse) when "Kalpana" is genuinely detected.
 */

#include "wake_detector.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

extern QueueHandle_t detection_queue;

static const char *TAG = "wake_detector";

// TODO: replace with real GPIO driver call (gpio_set_level) once pin is assigned
static void fire_wake_signal(uint32_t timestamp_ms)
{
    ESP_LOGI(TAG, "WAKE TRIGGERED at t=%ums — 'Kalpana' detected", timestamp_ms);
    // gpio_set_level(WAKE_GPIO_PIN, 1);
    // vTaskDelay(pdMS_TO_TICKS(10));
    // gpio_set_level(WAKE_GPIO_PIN, 0);
}

void wake_detector_task(void *pvParameters)
{
    wake_event_t evt;
    uint32_t last_trigger_ms = 0;

    for (;;) {
        if (xQueueReceive(detection_queue, &evt, portMAX_DELAY) == pdTRUE) {
            bool above_threshold = evt.confidence >= WAKE_CONFIDENCE_THRESHOLD;
            bool past_debounce   = (evt.timestamp_ms - last_trigger_ms) >= WAKE_DEBOUNCE_MS;

            if (above_threshold && past_debounce) {
                fire_wake_signal(evt.timestamp_ms);
                last_trigger_ms = evt.timestamp_ms;
            }
        }
    }
}
