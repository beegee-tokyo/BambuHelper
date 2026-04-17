#include "buzzer_backend.h"
#include "settings.h"
#include "config.h"

#if !defined(BOARD_HAS_ES8311_AUDIO)

static void sanitizeBuzzerPin() {
  if (buzzerSettings.pin == 0) return;
#if defined(BACKLIGHT_PIN)
  if (buzzerSettings.pin == BACKLIGHT_PIN) {
    Serial.printf("Buzzer: pin %d conflicts with backlight, disabling\n", buzzerSettings.pin);
    buzzerSettings.pin = 0;
  }
#endif
}

void buzzerBackendInit() {
  sanitizeBuzzerPin();
  if (buzzerSettings.pin == 0) return;
  pinMode(buzzerSettings.pin, OUTPUT);
  digitalWrite(buzzerSettings.pin, LOW);
}

void buzzerBackendApplyStep(uint16_t freq) {
  if (buzzerSettings.pin == 0) return;
  if (freq > 0) {
    tone(buzzerSettings.pin, freq);
  } else {
    noTone(buzzerSettings.pin);
    digitalWrite(buzzerSettings.pin, LOW);
  }
}

void buzzerBackendStop() {
  sanitizeBuzzerPin();
  if (buzzerSettings.pin == 0) return;
  noTone(buzzerSettings.pin);
  digitalWrite(buzzerSettings.pin, LOW);
}

void buzzerBackendTick() {
}

void buzzerBackendShutdown() {
  sanitizeBuzzerPin();
  buzzerBackendStop();
}

#endif // !BOARD_HAS_ES8311_AUDIO
