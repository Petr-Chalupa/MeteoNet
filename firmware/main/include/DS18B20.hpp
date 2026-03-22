#pragma once

#include "sensor.hpp"

class DS18B20 : ISensor<float> {
public:
  bool init() override { return false; }

  bool read(float &out) override { return false; }
};
