#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 7;

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
  static uint32_t lastPrinted = 0;
  const uint32_t current = interruptCount;
  if (current != lastPrinted) {
    Serial.printf("Interrupts: %lu (+%lu)\n", current, current - lastPrinted);
    lastPrinted = current;
  }
}
