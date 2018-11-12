#include "Button.h"

Button::Button(int pin) {
  this->pin = pin;
  pinMode(pin, INPUT_PULLUP);
}

bool Button::pressed() {
  return digitalRead(pin) == 0;
}
