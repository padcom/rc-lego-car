#include <Arduino.h>
#include "StatusIndicator.h"

StatusIndicator Status;

void StatusIndicator::begin() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void StatusIndicator::on() {
  digitalWrite(LED_BUILTIN, 0);
}

void StatusIndicator::off() {
  digitalWrite(LED_BUILTIN, 1);
}

void StatusIndicator::toggle() {
  digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == 0 ? 1 : 0);
}

bool StatusIndicator::state() {
  return digitalRead(LED_BUILTIN) == 0;
}
