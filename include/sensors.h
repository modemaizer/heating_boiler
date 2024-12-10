#pragma once
#include <cstdint>

void setupSensors();
void processSensors();
float getTemperature(std::uint8_t index);
