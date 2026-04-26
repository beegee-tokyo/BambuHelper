#include "led.h"
#include "settings.h"
#include "config.h"
#include <Arduino.h>

static int8_t attachedPin = -1;

bool isLedPinAllowed(uint8_t pin) {
  if (pin == 0) return false;

  // Generic dynamic conflicts (all boards)
  if (pin == BACKLIGHT_PIN) return false;
  if (buzzerSettings.enabled && pin == buzzerSettings.pin) return false;
  if (buttonType != BTN_DISABLED && pin == buttonPin) return false;

#if defined(DISPLAY_CYD)
  // CYD (classic ESP32, esp32dev)
  if (pin == 2 || pin == 12 || pin == 13 || pin == 14 || pin == 15) return false;  // display SPI
  if (pin == 25 || pin == 32 || pin == 33 || pin == 36 || pin == 39) return false; // XPT2046 touch
  if (pin == 4 || pin == 16 || pin == 17) return false;                            // onboard RGB
  if (pin == 26) return false;                                                     // onboard speaker amp
  if (pin >= 6 && pin <= 11) return false;                                         // SPI flash
  if (pin >= 34 && pin <= 39) return false;                                        // input-only
  if (pin > 39) return false;

#elif defined(BOARD_IS_S3)
  // LOLIN S3 mini + ST7789 240x240
  if (pin == 8 || pin == 9 || pin == 10 || pin == 11 || pin == 12) return false;   // display SPI
  if (pin == 19 || pin == 20) return false;                                        // USB CDC D-/D+
  if (pin >= 26 && pin <= 37) return false;                                        // SPI flash + PSRAM (qio_qspi)
  if (pin > 48) return false;

#elif defined(BOARD_IS_WS200)
  // Waveshare ESP32-S3-Touch-LCD-2.0"
  if (pin == 38 || pin == 39 || pin == 40 || pin == 42 || pin == 45) return false; // display SPI
  if (pin == 47 || pin == 48) return false;                                        // CST816D I2C
  if (pin == 19 || pin == 20) return false;                                        // USB CDC D-/D+
  if (pin >= 26 && pin <= 37) return false;                                        // SPI flash + PSRAM
  if (pin > 48) return false;

#elif defined(BOARD_IS_WS154)
  // Waveshare ESP32-S3-Touch-LCD-1.54"
  if (pin == 21 || pin == 38 || pin == 39 || pin == 40 || pin == 45) return false; // display SPI
  if (pin == 41 || pin == 42 || pin == 47 || pin == 48) return false;              // CST816 I2C + RST/IRQ
  if (pin == 8 || pin == 9 || pin == 10 || pin == 12) return false;                // ES8311 I2S
  if (pin == 7) return false;                                                      // audio PA ctrl
  if (pin == 2) return false;                                                      // BAT_EN
  if (pin == 0 || pin == 4 || pin == 5) return false;                              // board buttons
  if (pin == 19 || pin == 20) return false;                                        // USB CDC D-/D+
  if (pin >= 26 && pin <= 37) return false;                                        // SPI flash + PSRAM
  if (pin > 48) return false;

#elif defined(BOARD_IS_C3)
  // LOLIN C3 mini
  if (pin == 6 || pin == 7 || pin == 10 || pin == 20 || pin == 21) return false;   // display SPI
  if (pin == 18 || pin == 19) return false;                                        // USB CDC D-/D+
  if (pin >= 11 && pin <= 17) return false;                                        // flash/PSRAM
  if (pin > 21) return false;
#endif

  return true;
}

void sanitizeLedPin() {
  // Default unset state (disabled + pin=0) is valid — exit silently
  if (!ledSettings.enabled && ledSettings.pin == 0) return;
  if (!isLedPinAllowed(ledSettings.pin)) {
    Serial.printf("LED: pin %d not allowed, disabling\n", ledSettings.pin);
    ledSettings.enabled = false;
    ledSettings.pin = 0;
  }
}

static void detachAndForceLow() {
  if (attachedPin < 0) return;
  ledcWrite(LED_PWM_CH, 0);
  ledcDetachPin(attachedPin);
  pinMode(attachedPin, OUTPUT);
  digitalWrite(attachedPin, LOW);
  attachedPin = -1;
}

void initLed() {
  detachAndForceLow();
  // Disabled with a saved pin: drive it LOW instead of leaving it high-Z, so the
  // BJT/MOSFET gate is held off by firmware (independent of any external pulldown).
  // Only if the pin is allowed — never poke peripherals (display SPI, touch, etc.).
  if (!ledSettings.enabled) {
    if (ledSettings.pin != 0 && isLedPinAllowed(ledSettings.pin)) {
      pinMode(ledSettings.pin, OUTPUT);
      digitalWrite(ledSettings.pin, LOW);
    }
    return;
  }
  if (!isLedPinAllowed(ledSettings.pin)) return;
  ledcSetup(LED_PWM_CH, LED_PWM_FREQ, LED_PWM_RES);
  ledcAttachPin(ledSettings.pin, LED_PWM_CH);
  attachedPin = ledSettings.pin;
  ledcWrite(LED_PWM_CH, ledSettings.brightness);
}

void shutdownLed() {
  detachAndForceLow();
}

void commitLedBrightness(uint8_t brightness) {
  ledSettings.brightness = brightness;
  if (attachedPin >= 0 && ledSettings.enabled) {
    ledcWrite(LED_PWM_CH, brightness);
  }
}

void applyLedDuty(uint8_t duty) {
  if (attachedPin >= 0 && ledSettings.enabled) {
    ledcWrite(LED_PWM_CH, duty);
  }
}

void restoreLedDuty() {
  if (attachedPin >= 0 && ledSettings.enabled) {
    ledcWrite(LED_PWM_CH, ledSettings.brightness);
  }
}

void previewLed(bool enabled, uint8_t pin, uint8_t brightness) {
  if (!enabled || !isLedPinAllowed(pin)) {
    detachAndForceLow();
    return;
  }
  if (attachedPin != (int8_t)pin) {
    detachAndForceLow();
    ledcSetup(LED_PWM_CH, LED_PWM_FREQ, LED_PWM_RES);
    ledcAttachPin(pin, LED_PWM_CH);
    attachedPin = pin;
  }
  ledcWrite(LED_PWM_CH, brightness);
}
