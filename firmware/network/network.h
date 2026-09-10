#ifndef NETWORK_H
#define NETWORK_H

/**
 * FreeRTOS task: reports wake events and periodic health/status pings over
 * UART (and optionally Wi-Fi/BLE, depending on final integration target).
 * Kept independent of the audio/inference/detection pipeline so activation
 * remains fully on-device even if this task stalls or is disabled.
 */
void network_task(void *pvParameters);

int network_init(void);

#endif // NETWORK_H
