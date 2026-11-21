#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 7;
constexpr uint32_t POLL_INTERVAL_MS = 10;
constexpr uint32_t DEBOUNCE_MS = 50;

class DebouncedButton {
public:
  explicit DebouncedButton(uint8_t pin) : pin(pin) {}

  void begin() {
    pinMode(pin, INPUT);
  }

  bool update(uint32_t nowMs) {
    const bool isLow = digitalRead(pin) == LOW;
    switch (state) {
      case State::Released:
        if (isLow) {
          state = State::MaybePressed;
          stateSinceMs = nowMs;
        }
        return false;

      case State::MaybePressed:
        if (!isLow) {
          state = State::Released;
          return false;
        }
        if (nowMs - stateSinceMs < DEBOUNCE_MS) {
          return false;
        }
        state = State::Pressed;
        return true;

      case State::Pressed:
        if (!isLow) {
          state = State::MaybeReleased;
          stateSinceMs = nowMs;
        }
        return false;

      case State::MaybeReleased:
        if (isLow) {
          state = State::Pressed;
        } else if (nowMs - stateSinceMs >= DEBOUNCE_MS) {
          state = State::Released;
        }
        return false;
    }
    return false;
  }

private:
  enum class State { Released, MaybePressed, Pressed, MaybeReleased };

  const uint8_t pin;
  State state = State::Released;
  uint32_t stateSinceMs = 0;
};

DebouncedButton button(BUTTON_PIN);

void setup() {
  Serial.begin(115200);
  button.begin();
  Serial.println("Ready. Press the button.");
}

void loop() {
  static uint32_t lastPollMs = 0;
  static uint32_t pressCount = 0;

  const uint32_t nowMs = millis();
  if (nowMs - lastPollMs < POLL_INTERVAL_MS) {
    return;
  }
  lastPollMs = nowMs;

  if (button.update(nowMs)) {
    pressCount++;
    Serial.printf("Pressed: %lu\n", pressCount);
  }
}
