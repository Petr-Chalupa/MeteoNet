#pragma once

#include "sensor.hpp"

struct BME280Data {
  float temperature;
  float humidity;
  float pressure;
};

class BME280 : ISensor<BME280Data> {
public:
  bool init() override { return false; }

  bool read(BME280Data &out) override { return false; }
};