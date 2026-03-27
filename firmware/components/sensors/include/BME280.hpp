#pragma once

#include "bme280.h"
#include "driver/i2c_master.h"
#include "sensor.hpp"

struct BME280Data {
  float temperature;
  float humidity;
  float pressure;
};

class BME280 : ISensor<BME280Data> {
  i2c_master_bus_handle_t _bus_handle = nullptr;
  bme280_handle_t _dev_handle = nullptr;

public:
  BME280(i2c_master_bus_handle_t bus) : _bus_handle(bus) {}

  bool init() override;

  void deinit() override;

  bool read(BME280Data &out) override;
};