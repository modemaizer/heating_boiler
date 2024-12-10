#include <Arduino.h>

#include "defines.h"
#include "creds.h"
#include "wifi_connector.h"
#include "sensors.h"
#include "display.h"
#include "mqtt.h"

void onWifiConnected() {
  setupMdns(MDNS_NAME);
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("Start");
  wifiInit();
  setupSensors();
  setupDisplay();
  setupMqtt();
}

void loop() {
  processSensors();
  printHeaterTemperatures();
  processMqtt();
}