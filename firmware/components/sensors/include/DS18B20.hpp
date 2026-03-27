#pragma once

#include "sensor.hpp"

class DS18B20 : ISensor<float> {
public:
  bool init() override;

  void deinit() override;

  bool read(float &out) override;
};
