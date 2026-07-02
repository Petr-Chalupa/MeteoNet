#include "DS18B20.hpp"
#include "ds18b20.h"
#include "onewire_bus.h"

bool DS18B20::init() {
  onewire_device_iter_handle_t iter = NULL;
  onewire_device_t next_dev;

  if (onewire_new_device_iter(_bus_handle, &iter) != ESP_OK) return false;

  if (onewire_device_iter_get_next(iter, &next_dev) == ESP_OK) {
    ds18b20_config_t ds_cfg = {};

    if (ds18b20_new_device_from_enumeration(&next_dev, &ds_cfg, &_dev_handle) == ESP_OK) {
      onewire_del_device_iter(iter);
      return true;
    }
  }

  onewire_del_device_iter(iter);
  return false;
}

void DS18B20::deinit() {
  if (!_dev_handle) return;

  ds18b20_del_device(_dev_handle);
  _dev_handle = nullptr;
}

bool DS18B20::read(float &out) {
  if (_dev_handle == NULL) return false;

  if (ds18b20_trigger_temperature_conversion(_dev_handle) != ESP_OK) return false;

  return (ds18b20_get_temperature(_dev_handle, &out) == ESP_OK);
}
