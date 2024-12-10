#include <Arduino.h>
#include <GyverOLED.h>
// пример компактного асинхронного опроса датчиков на программном таймере
// https://alexgyver.ru/lessons/time/

#define DS_PIN  D3

uint8_t s1_addr[] {0x28, 0x61, 0x64, 0xA, 0xB6, 0x35, 0xF5, 0xB};
uint8_t s2_addr[] {0x28, 0x61, 0x64, 0xA, 0xB6, 0x38, 0xA3, 0x84};
uint8_t s3_addr[] {0x28, 0xFF, 0x61, 0x75, 0xD0, 0x1, 0x3C, 0x7D};
uint8_t s4_addr[] {0x28, 0x61, 0x64, 0xA, 0xB1, 0xDE, 0x1D, 0xC};
uint8_t s5_addr[] {0x28, 0x1C, 0x66, 0x44, 0xD4, 0xE1, 0x3C, 0x70};
uint8_t s6_addr[] {0x28, 0xE1, 0xD7, 0x75, 0xD0, 0x1, 0x3C, 0xE6};
uint8_t s7_addr[] {0x28, 0xD1, 0xDD, 0x44, 0xD4, 0xE1, 0x3C, 0x9A};
uint8_t s8_addr[] {0x28, 0x61, 0x64, 0xA, 0xB6, 0xE, 0xBB, 0x5C};
uint8_t s9_addr[] {0x28, 0x61, 0x64, 0xA, 0xB3, 0x75, 0x89, 0xFE};

#include <microDS18B20.h>

MicroDS18B20<DS_PIN, s1_addr> heater_out_s;
MicroDS18B20<DS_PIN, s2_addr> heater_in_s;
MicroDS18B20<DS_PIN, s3_addr> sensor3;
MicroDS18B20<DS_PIN, s4_addr> sensor4;
MicroDS18B20<DS_PIN, s5_addr> sensor5;

GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;

float heater_out_t = 0;
float heater_in_t = 0;

void setup() {
  Serial.begin(115200);
  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(2);
}

void loop() {
  // конструкция программного таймера на 1c
  static uint32_t tmr;
  if (millis() - tmr >= 1000) {
    tmr = millis();
    oled.home();
    // выводим показания в порт
    Serial.print("Котёл подача: ");
    oled.print("Up: ");
    if (heater_out_s.readTemp()) {
      heater_out_t = heater_out_s.getTemp();
      Serial.println(heater_out_t);
    }
    else {
      heater_out_t = 0;
      Serial.println("error");
    }
    heater_out_s.requestTemp();
    oled.print(heater_out_t);
    oled.setCursor(0, 2);

    Serial.print("Котёл обратка: ");
    oled.print("Down: ");
    if (heater_in_s.readTemp()) {
      heater_in_t = heater_in_s.getTemp();
      Serial.println(heater_in_t);
    }
    else {
      heater_in_t = 0;
      Serial.println("error");
    }
    heater_in_s.requestTemp();
    oled.print(heater_in_t);

    Serial.print("t3: ");
    if (sensor3.readTemp()) Serial.println(sensor3.getTemp());
    else Serial.println("error");

    sensor3.requestTemp();

    Serial.print("t4: ");
    if (sensor4.readTemp()) Serial.println(sensor4.getTemp());
    else Serial.println("error");

    sensor4.requestTemp();

    Serial.print("t5: ");
    if (sensor5.readTemp()) Serial.println(sensor5.getTemp());
    else Serial.println("error");

    sensor5.requestTemp();


  }
}