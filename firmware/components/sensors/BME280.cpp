#include "BME280.hpp"
#include "bme280.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cstring>

int8_t BME280::read_reg(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  auto dev_handle = static_cast<i2c_master_dev_handle_t>(intf_ptr);

  esp_err_t err = i2c_master_transmit_receive(dev_handle, &reg_addr, 1, reg_data, len, -1);
  return (err == ESP_OK) ? 0 : -1;
}

int8_t BME280::write_reg(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  auto dev_handle = static_cast<i2c_master_dev_handle_t>(intf_ptr);

  uint8_t buffer[len + 1];
  buffer[0] = reg_addr;
  memcpy(&buffer[1], reg_data, len);

  esp_err_t err = i2c_master_transmit(dev_handle, buffer, len + 1, -1);
  return (err == ESP_OK) ? 0 : -1;
}

void BME280::delay_us(uint32_t period, void *intf_ptr) {
  uint32_t ms = (period / 1000) + 1;
  vTaskDelay(pdMS_TO_TICKS(ms));
}

bool BME280::init() {
  _dev_ptr.intf = BME280_I2C_INTF;
  _dev_ptr.read = BME280::read_reg;
  _dev_ptr.write = BME280::write_reg;
  _dev_ptr.delay_us = BME280::delay_us;
  _dev_ptr.intf_ptr = _dev_handle;

  if (bme280_init(&_dev_ptr) != BME280_OK) return false;

  struct bme280_settings settings;
  bme280_get_sensor_settings(&settings, &_dev_ptr);
  settings.osr_h = BME280_OVERSAMPLING_1X;
  settings.osr_p = BME280_OVERSAMPLING_1X;
  settings.osr_t = BME280_OVERSAMPLING_1X;
  settings.filter = BME280_FILTER_COEFF_OFF;
  uint8_t settings_sel = BME280_SEL_OSR_PRESS | BME280_SEL_OSR_TEMP | BME280_SEL_OSR_HUM | BME280_SEL_FILTER;
  bme280_set_sensor_settings(settings_sel, &settings, &_dev_ptr);

  bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &_dev_ptr);

  return true;
}

void BME280::deinit() { bme280_set_sensor_mode(BME280_POWERMODE_SLEEP, &_dev_ptr); }

bool BME280::read(BME280Data &out) {
  struct bme280_data comp_data;

  if (bme280_get_sensor_data(BME280_ALL, &comp_data, &_dev_ptr) != BME280_OK) return false;

  out.temperature = comp_data.temperature;
  out.pressure = comp_data.pressure / 100.0f;
  out.humidity = comp_data.humidity;

  return true;
}
