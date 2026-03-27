#pragma once

#include "ds18b20.h"
#include "onewire_bus.h"
#include "sensor.hpp"

class DS18B20 : ISensor<float> {
  onewire_bus_handle_t _bus_handle;
  ds18b20_device_handle_t _dev_handle;

public:
  DS18B20(onewire_bus_handle_t bus) : _bus_handle(bus) {}

  bool init() override;

  void deinit() override;

  bool read(float &out) override;
};
