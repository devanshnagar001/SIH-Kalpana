/*
 * SIH26172 — Firmware Skeleton (Week 1)
 * Two-task structure: Audio Capture + Inference, connected by a queue.
 *
 * NOTE: Both tasks are currently STUBS.
 * - Audio task: P2 will replace the stub with real I2S mic reading.
 * - Inference task: P4 will replace the stub with real MFCC + model logic.
 * Contract values below match Contract v1 (frozen 26 Aug) — do not change
 * without updating README.md and announcing in the group chat.
 */

// ---------- Contract constants (from Contract v1) ----------
#define SAMPLE_RATE      16000
#define FRAME_SIZE       480     // 30 ms window @ 16kHz
#define HOP_SIZE         320     // 20 ms hop @ 16kHz
#define QUEUE_LENGTH     10      // how many audio chunks can wait in the queue
                                  // before the inference task falls behind

// ---------- Shared queue between the two tasks ----------
// This is the "pipe" audio chunks travel through from Task A to Task B.
QueueHandle_t audioQueue;

// A placeholder struct representing one chunk of audio handed off.
// P2 will define the real version of this once the ring buffer exists.
typedef struct {
  int16_t samples[FRAME_SIZE];
  uint32_t timestamp;
} AudioChunk;


// ---------- TASK 1: Audio Capture (stub) ----------
// Real job (P2): continuously read I2S mic data into a ring buffer,
// then push chunks into audioQueue.
void audioTask(void *parameter) {
  AudioChunk chunk;

  while (true) {
    // --- STUB: fill with fake data for now ---
    memset(chunk.samples, 0, sizeof(chunk.samples));
    chunk.timestamp = millis();

    // Push chunk into the queue for the inference task to pick up.
    // portMAX_DELAY = wait as long as needed if the queue is full
    // (in the real version, P2 should use a short timeout instead,
    // so audio capture never stalls waiting on inference).
    xQueueSend(audioQueue, &chunk, portMAX_DELAY);

    Serial.println("[Audio] chunk sent");

    // Stub pacing only — real version paces itself off actual I2S reads,
    // not a delay.
    vTaskDelay(pdMS_TO_TICKS(20)); // ~ matches 20ms hop for now
  }
}


// ---------- TASK 2: Inference (stub) ----------
// Real job (P4/P3): pull audio chunks, run MFCC + model, check for "Kalpana".
void inferenceTask(void *parameter) {
  AudioChunk chunk;

  while (true) {
    // Wait here until a chunk arrives from the audio task.
    if (xQueueReceive(audioQueue, &chunk, portMAX_DELAY) == pdTRUE) {

      // --- STUB: pretend to process it ---
      Serial.print("[Inference] received chunk at t=");
      Serial.println(chunk.timestamp);

      // Real version: run MFCC -> model -> check posterior threshold
      // (3 consecutive posteriors > 0.90, per Contract v1)
    }
  }
}


// ---------- Setup: create the queue and both tasks ----------
void setup() {
  Serial.begin(115200);
  delay(1000); // let Serial settle
  Serial.println("SIH26172 firmware skeleton starting...");

  // Create the queue: holds up to QUEUE_LENGTH AudioChunk items
  audioQueue = xQueueCreate(QUEUE_LENGTH, sizeof(AudioChunk));

  if (audioQueue == NULL) {
    Serial.println("ERROR: failed to create audioQueue");
    while (true); // halt — nothing works without the queue
  }

  // Create the two tasks.
  // xTaskCreate(function, name, stack size, params, priority, task handle)
  xTaskCreate(
    audioTask,        // function to run
    "AudioTask",      // name (for debugging)
    4096,             // stack size in bytes — adjust if you see stack overflow warnings
    NULL,              // no parameters passed in
    2,                 // priority — HIGHER than inference (audio must not be delayed)
    NULL               // don't need a handle to this task right now
  );

  xTaskCreate(
    inferenceTask,
    "InferenceTask",
    4096,
    NULL,
    1,                 // priority — lower than audio task
    NULL
  );

  Serial.println("Both tasks created.");
}

// loop() stays empty — FreeRTOS tasks handle everything now,
// so the default Arduino loop isn't used.
void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
