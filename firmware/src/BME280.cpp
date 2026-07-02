#include "sensor.hpp"
#include <Adafruit_BME280.h>
#include <Wire.h>

#define SDA 21
#define SCL 22
#define ADDRESS 0x76

struct BME280Data {
  float temperature;
  float humidity;
  float pressure;
};

class BME280Sensor : public ISensor<BME280Data> {
private:
  Adafruit_BME280 bme;
  bool initialized = false;

public:
  bool init() override {
    Wire.begin(SDA, SCL);
    initialized = bme.begin(ADDRESS);

    return initialized;
  }

  void deinit() override {
    pinMode(SDA, INPUT);
    pinMode(SCL, INPUT);
    Wire.end();

    initialized = false;
  }

  bool read(BME280Data &out) override {
    if (!initialized) return false;

    out.temperature = bme.readTemperature();
    out.humidity = bme.readHumidity();
    out.pressure = bme.readPressure() / 100.0F;

    return true;
  }
};