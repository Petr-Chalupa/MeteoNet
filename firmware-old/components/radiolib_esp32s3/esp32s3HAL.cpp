#include "esp32s3HAL.hpp"
#include <driver/gpio.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LOW (0x0)
#define HIGH (0x1)
#define INPUT (0x01)
#define OUTPUT (0x03)
#define RISING (0x01)
#define FALLING (0x02)
#define NOP() asm volatile("nop")

Esp32s3Hal::Esp32s3Hal(spi_host_device_t host, int sck, int miso, int mosi)
    : RadioLibHal(INPUT, OUTPUT, LOW, HIGH, RISING, FALLING), _host(host), _sck(sck), _miso(miso), _mosi(mosi) {}

void Esp32s3Hal::pinMode(uint32_t pin, uint32_t mode) {
  if (pin == RADIOLIB_NC) return;

  gpio_mode_t m = (mode == OUTPUT) ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT;
  gpio_config_t conf{};
  conf.pin_bit_mask = 1ULL << pin;
  conf.mode = m;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&conf);
}

void Esp32s3Hal::digitalWrite(uint32_t pin, uint32_t value) {
  if (pin == RADIOLIB_NC) return;
  gpio_set_level((gpio_num_t)pin, value);
}

uint32_t Esp32s3Hal::digitalRead(uint32_t pin) {
  if (pin == RADIOLIB_NC) return 0;
  return gpio_get_level((gpio_num_t)pin);
}

void Esp32s3Hal::attachInterrupt(uint32_t pin, void (*cb)(void), uint32_t mode) {
  if (pin == RADIOLIB_NC) return;

  static bool installed = false;
  if (!installed) {
    gpio_install_isr_service(0);
    installed = true;
  }

  gpio_set_intr_type((gpio_num_t)pin, (mode & RISING) ? GPIO_INTR_POSEDGE : GPIO_INTR_NEGEDGE);

  gpio_isr_handler_add((gpio_num_t)pin, (gpio_isr_t)cb, nullptr);
}

void Esp32s3Hal::detachInterrupt(uint32_t pin) {
  if (pin == RADIOLIB_NC) return;
  gpio_isr_handler_remove((gpio_num_t)pin);
  gpio_set_intr_type((gpio_num_t)pin, GPIO_INTR_DISABLE);
}

long Esp32s3Hal::pulseIn(uint32_t pin, uint32_t state, RadioLibTime_t timeout) {
  if (pin == RADIOLIB_NC) return 0;

  pinMode(pin, INPUT);
  uint64_t start = esp_timer_get_time();
  uint64_t elapsed = 0;

  // Wait for pulse to start
  while (digitalRead(pin) == state) {
    elapsed = esp_timer_get_time() - start;
    if (elapsed > timeout) return 0;
  }

  start = esp_timer_get_time();
  // Wait for pulse to end
  while (digitalRead(pin) != state) {
    elapsed = esp_timer_get_time() - start;
    if (elapsed > timeout) return 0;
  }

  return esp_timer_get_time() - start;
}

void Esp32s3Hal::delay(RadioLibTime_t ms) { vTaskDelay(ms / portTICK_PERIOD_MS); }

void Esp32s3Hal::delayMicroseconds(RadioLibTime_t us) {
  uint64_t start = esp_timer_get_time();
  while ((esp_timer_get_time() - start) < us) {
    NOP();
  }
}

uint32_t Esp32s3Hal::millis() { return static_cast<uint32_t>(esp_timer_get_time() / 1000ULL); }

uint32_t Esp32s3Hal::micros() { return static_cast<uint32_t>(esp_timer_get_time()); }

void Esp32s3Hal::spiBegin() {
  spi_bus_config_t buscfg{};
  buscfg.miso_io_num = _miso;
  buscfg.mosi_io_num = _mosi;
  buscfg.sclk_io_num = _sck;
  buscfg.quadwp_io_num = -1;
  buscfg.quadhd_io_num = -1;
  spi_bus_initialize(_host, &buscfg, SPI_DMA_CH_AUTO);

  spi_device_interface_config_t devcfg{};
  devcfg.clock_speed_hz = 2 * 1000 * 1000; // 2 MHz
  devcfg.mode = 0;
  devcfg.spics_io_num = -1;
  devcfg.queue_size = 1;

  spi_bus_add_device(_host, &devcfg, &_spi);
}

void Esp32s3Hal::spiBeginTransaction() {}

void Esp32s3Hal::spiTransfer(uint8_t *out, size_t len, uint8_t *in) {
  spi_transaction_t t{};
  t.length = len * 8;
  t.tx_buffer = out;
  t.rx_buffer = in;
  spi_device_transmit(_spi, &t);
}

void Esp32s3Hal::spiEndTransaction() {}

void Esp32s3Hal::spiEnd() {
  spi_bus_remove_device(_spi);
  spi_bus_free(_host);
}