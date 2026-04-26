#ifndef LED_H
#define LED_H

#include <Arduino.h>

// Lifecycle
void initLed();
void shutdownLed();

// Configured-brightness path (slider Save). Persists in caller via saveLedSettings().
void commitLedBrightness(uint8_t brightness);

// Transient-duty path for future effects (flash, ramp). Does not modify ledSettings.
void applyLedDuty(uint8_t duty);
void restoreLedDuty();

// Pin validation. sanitizeLedPin() is called from saveLedSettings() before NVS write.
void sanitizeLedPin();
bool isLedPinAllowed(uint8_t pin);

// Live preview from web UI: reconfigures LED with form values. NVS untouched.
void previewLed(bool enabled, uint8_t pin, uint8_t brightness);

#endif // LED_H
