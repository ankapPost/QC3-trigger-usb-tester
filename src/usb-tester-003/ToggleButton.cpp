#include "Arduino.h"

class ToggleButton {
  int buttonState;
  int lastButtonState = HIGH;
  int pin;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;
  typedef void (*callback_t)(ToggleButton *btn);
  callback_t callback;

  public:  ToggleButton(int p, callback_t cb) {
      pin = p;
      pinMode(pin, INPUT_PULLUP);
    //    this->_onPressCallback = callback;
      callback = cb;
  }
  public: void read() {
    int reading = digitalRead(pin);
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
        if (buttonState == LOW) {
          if (this->callback) this->callback(this);
        }
      }
    }
    lastButtonState = reading;
/*
    */
    }
};
