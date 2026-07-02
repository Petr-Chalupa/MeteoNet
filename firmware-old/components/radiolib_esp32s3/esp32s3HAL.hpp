#pragma once

#include <RadioLib.h>
#include <driver/spi_master.h>

class Esp32s3Hal : public RadioLibHal {
  spi_host_device_t _host;
  spi_device_handle_t _spi;

  int _sck;
  int _miso;
  int _mosi;

public:
  Esp32s3Hal(spi_host_device_t host, int sck, int miso, int mosi);

  void pinMode(uint32_t pin, uint32_t mode) override;
  void digitalWrite(uint32_t pin, uint32_t value) override;
  uint32_t digitalRead(uint32_t pin) override;

  void delay(RadioLibTime_t ms) override;
  void delayMicroseconds(RadioLibTime_t us) override;

  uint32_t millis() override;
  uint32_t micros() override;

  void spiBegin() override;
  void spiBeginTransaction() override;
  void spiTransfer(uint8_t *out, size_t len, uint8_t *in) override;
  void spiEndTransaction() override;
  void spiEnd() override;

  void attachInterrupt(uint32_t pin, void (*cb)(void), uint32_t mode) override;
  void detachInterrupt(uint32_t pin) override;
  long pulseIn(uint32_t pin, uint32_t state, RadioLibTime_t timeout) override;

  void init() override { spiBegin(); }
  void term() override { spiEnd(); }
};