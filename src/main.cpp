#include <Arduino.h>

#include "sensors.h"
#include "display.h"

void setup() {
  Serial.begin(115200);
  setupSensors();
  setupDisplay();
}

void loop() {
  processSensors();
  printHeaterTemperatures();
}