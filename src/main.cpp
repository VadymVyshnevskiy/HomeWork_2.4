#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 7;
constexpr uint32_t DEBOUNCE_MS = 50;

volatile uint32_t interruptCount = 0;

void IRAM_ATTR onButtonFalling() {
  interruptCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonFalling, FALLING);
  Serial.println("Ready. Press the button.");
}

void loop() {
  static uint32_t lastSeenInterrupts = 0;
  static uint32_t lastAcceptedMs = 0;
  static uint32_t acceptedCount = 0;

  const uint32_t current = interruptCount;
  if (current == lastSeenInterrupts) {
    return;
  }
  lastSeenInterrupts = current;

  const uint32_t nowMs = millis();
  if (nowMs - lastAcceptedMs < DEBOUNCE_MS) {
    return;
  }
  lastAcceptedMs = nowMs;
  acceptedCount++;
  Serial.printf("Accepted: %lu | raw interrupts: %lu\n", acceptedCount, current);
}
