#include "button.h"
#include "settings.h"
#ifdef TOUCH_CS
  #include "display_ui.h"  // extern tft for getTouch()
#endif

static bool lastRaw = false;
static bool stableState = false;
static unsigned long lastChangeMs = 0;
static const unsigned long DEBOUNCE_MS = 50;

void initButton() {
  if (buttonType == BTN_DISABLED) return;
  if (buttonType == BTN_TOUCHSCREEN) return;  // TFT_eSPI handles SPI
  if (buttonPin == 0) return;
  if (buttonType == BTN_PUSH) {
    pinMode(buttonPin, INPUT_PULLUP);
  } else {  // BTN_TOUCH (TTP223)
    pinMode(buttonPin, INPUT);
  }
  lastRaw = false;
  stableState = false;
  lastChangeMs = 0;
}

bool wasButtonPressed() {
  if (buttonType == BTN_DISABLED) return false;

  bool raw;
  if (buttonType == BTN_TOUCHSCREEN) {
#ifdef TOUCH_CS
    uint16_t tx, ty;
    raw = tft.getTouch(&tx, &ty);
#else
    return false;
#endif
  } else if (buttonType == BTN_PUSH) {
    if (buttonPin == 0) return false;
    raw = (digitalRead(buttonPin) == LOW);   // active LOW with pull-up
  } else {
    if (buttonPin == 0) return false;
    raw = (digitalRead(buttonPin) == HIGH);  // TTP223: active HIGH
  }

  // Debounce
  if (raw != lastRaw) {
    lastChangeMs = millis();
    lastRaw = raw;
  }
  if ((millis() - lastChangeMs) < DEBOUNCE_MS) return false;

  // Rising edge detection
  bool result = false;
  if (raw && !stableState) {
    result = true;
  }
  stableState = raw;

  return result;
}
