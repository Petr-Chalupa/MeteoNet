#include "sensor.hpp"
#include <DallasTemperature.h>
#include <OneWire.h>

class DS18B20Sensor : public ISensor<float> {
private:
  uint8_t pin;
  OneWire oneWire;
  DallasTemperature sensors;
  bool initialized = false;

public:
  explicit DS18B20Sensor(uint8_t dataPin) : pin(dataPin), oneWire(dataPin), sensors(&oneWire) {}

  bool init() override {
    sensors.begin();

    if (sensors.getDeviceCount() == 0) {
      initialized = false;
      return false;
    }

    initialized = true;
    return true;
  }

  void deinit() override {
    pinMode(pin, INPUT);

    initialized = false;
  }

  bool read(float &out) override {
    if (!initialized) return false;

    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);

    if (temp == DEVICE_DISCONNECTED_C) return false;

    out = temp;

    return true;
  }
};