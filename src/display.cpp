#include <GyverOLED.h>

#include "defines.h"
#include "sensors.h"

GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;

void setupDisplay() {
  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(2);   // размер шрифта
}

void printHeaterTemperatures() {
  oled.home();
  oled.print("OUT: ");
  oled.print(getTemperature(HEATER_OUT_INDEX));
  oled.setCursor(0, 2);
  oled.print("IN:  ");
  oled.print(getTemperature(HEATER_IN_INDEX));
}