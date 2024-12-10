#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include "wifi_connector.h"
#include "defines.h"
#include "sensors.h"
#include "mqtt.h"
#include "creds.h"

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);
static const char* mqttServer = MQTT_SERVER;
static const int mqttPort = MQTT_PORT;

uint32_t mqttConnectTimer = 0;
uint32_t mqttSendTimer = 0;

boolean reconnect() {
  if (mqttClient.connect(MQTT_ID, MQTT_USER, MQTT_PASS)) {
    mqttClient.subscribe(MQTT_COMMAND_TOPIC);
    mqttSendDeviceState();
  }
  return mqttClient.connected();
}

void processMqtt()
{
  if (!mqttClient.connected() && (!mqttConnectTimer || millis() - mqttConnectTimer > MQTT_RECONNECT_TIMEOUT)) {
    mqttConnectTimer = millis();
    reconnect();
  }
  if (mqttClient.connected() && (!mqttSendTimer || millis() - mqttSendTimer > MQTT_INTERVAL)) {
    mqttSendTimer = millis();
    mqttSendDeviceState();
  }
  mqttClient.loop();
}

static void parseIncomingCommand(char *topic, byte *payload, unsigned int length)
{
  char command[length + 1];
  for (uint32_t i = 0; i < length; i++)
    command[i] = (char)payload[i];

  command[length] = '\0';

  if (strcmp(command, "restart") == 0) {
    ESP.restart();
  }
  
  mqttPrintf(MQTT_LOG_TOPIC, "%s", command);
}

void setupMqtt()
{
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback([](char *topic, byte *payload, unsigned int length)
             { parseIncomingCommand(topic, payload, length); });
}

// example: mqttPrintf("topic", "Hello, %s! The answer is %d", "World", 42);
void mqttPrintf(const char *topic, const char *format, ...)
{
  uint16_t bufferSize = 512;
  char* buffer = new char[bufferSize];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, bufferSize, format, args);
  va_end(args);
  mqttClient.publish(topic, buffer);
  delete [] buffer;
}

void mqttSendDeviceState() {
  DynamicJsonDocument doc(512);
  
  doc["boilerOut"] = getTemperature(HEATER_OUT_INDEX);
  doc["boilerIn"] = getTemperature(HEATER_IN_INDEX);
  doc["exchangerFromBoiler"] = getTemperature(EXCHANGER_FROM_HEATER_INDEX);
  doc["exchangerToBoiler"] = getTemperature(EXCHANGER_TO_HEATER_INDEX);
  doc["exchangerFromCollector"] = getTemperature(EXCHANGER_FROM_COLLECTOR_INDEX);
  doc["exchangerToCollector"] = getTemperature(EXCHANGER_TO_COLLECTOR_INDEX);
  doc["collectorIn"] = getTemperature(COLLECTOR_IN_INDEX);
  doc["collectorOut"] = getTemperature(COLLECTOR_OUT_INDEX);
  doc["thermostat"] = getTemperature(THERMOSTAT_INDEX);
  
  char buffer[512];
  size_t n = serializeJson(doc, buffer);
  mqttClient.publish(MQTT_JSON_TOPIC, buffer, n);
}
