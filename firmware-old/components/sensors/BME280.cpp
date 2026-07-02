#include "BME280.hpp"
#include "bme280.h"
#include "driver/i2c_master.h"

bool BME280::init() {
  _dev_handle = bme280_create(_bus_handle, BME280_I2C_ADDRESS_DEFAULT);

  if (_dev_handle == NULL) return false;

  return (bme280_default_init(_dev_handle) == ESP_OK);
}

void BME280::deinit() {
  if (!_dev_handle) return;

  bme280_delete(&_dev_handle);
  _dev_handle = nullptr;
}

bool BME280::read(BME280Data &out) {
  if (!_dev_handle) return false;

  esp_err_t err_temp = bme280_read_temperature(_dev_handle, &out.temperature);
  esp_err_t err_pres = bme280_read_pressure(_dev_handle, &out.pressure);
  esp_err_t err_humi = bme280_read_humidity(_dev_handle, &out.humidity);

  return (err_temp == ESP_OK && err_pres == ESP_OK && err_humi == ESP_OK);
}
