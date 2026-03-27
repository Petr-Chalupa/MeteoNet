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
  i2c_master_dev_handle_t _dev_handle;
  bme280_dev _dev_ptr;

  static int8_t read_reg(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

  static int8_t write_reg(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);

  static void delay_us(uint32_t period, void *intf_ptr);

public:
  bool init() override;

  void deinit() override;

  bool read(BME280Data &out) override;
};