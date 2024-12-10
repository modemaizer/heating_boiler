#include <microDS18B20.h>

#include "defines.h"

#define DS_SENSOR_AMOUNT 9

const uint8_t addr[][8] PROGMEM = {
  {0x28, 0x61, 0x64, 0xA, 0xB6, 0x35, 0xF5, 0xB},   // Котёл подача
  {0x28, 0x61, 0x64, 0xA, 0xB6, 0x38, 0xA3, 0x84},  // Котёл обратка
  {0x28, 0xFF, 0x61, 0x75, 0xD0, 0x1, 0x3C, 0x7D},  // Теплообменник от котла
  {0x28, 0x61, 0x64, 0xA, 0xB1, 0xDE, 0x1D, 0xC},   // Теплообменник к котлу
  {0x28, 0x1C, 0x66, 0x44, 0xD4, 0xE1, 0x3C, 0x70}, // Теплообменник от коллектора
  {0x28, 0xE1, 0xD7, 0x75, 0xD0, 0x1, 0x3C, 0xE6},  // Теплообменник к коллектору
  {0x28, 0xD1, 0xDD, 0x44, 0xD4, 0xE1, 0x3C, 0x9A}, // Коллектор подача
  {0x28, 0x61, 0x64, 0xA, 0xB6, 0xE, 0xBB, 0x5C},   // Коллектор обратка
  {0x28, 0x61, 0x64, 0xA, 0xB3, 0x75, 0x89, 0xFE}   // Термостат
};

#include <microDS18B20.h>

MicroDS18B20<DS_PIN, DS_ADDR_MODE, DS_SENSOR_AMOUNT, DS_PROGMEM> sensors;

float temperatures[DS_SENSOR_AMOUNT];

void setupSensors() {
  sensors.setAddress((uint8_t*)addr);

  // Установить разрешение 9 бит у всех датчиков на линии
  // Точность 0.5 градуса
  // Не работает
  sensors.setResolutionAll(9);
}

void processSensors() {
  static uint32_t tmr;
  if (millis() - tmr >= 1000) {
    tmr = millis();
    
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
      temperatures[i] = sensors.getTemp(i);
    }
    // и тут не работает ((
    sensors.setResolutionAll(9);
    sensors.requestTempAll();
  }
}

float getTemperature(uint8_t index) {
  return temperatures[index];
}

