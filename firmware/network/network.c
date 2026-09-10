/**
 * network.c
 * Status/reporting channel — UART heartbeat for now, placeholder for a
 * future Wi-Fi/BLE link to a host system. Not on the critical wake-word
 * detection path (see contract_v1.md — activation must stay on-device).
 */

#include "network.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "network";

int network_init(void)
{
    // TODO: init UART (and optionally Wi-Fi/BLE) per final integration target
    ESP_LOGI(TAG, "Network/status channel initialized (stub)");
    return 0;
}

void network_task(void *pvParameters)
{
    network_init();

    for (;;) {
        ESP_LOGI(TAG, "heartbeat: kalpana firmware alive");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
