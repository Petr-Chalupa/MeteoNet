#include "lora.hpp"
#include "esp32s3HAL.hpp"
#include <RadioLib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LORA_HOST SPI2_HOST
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SCK 5
#define LORA_CS 18
#define LORA_DIO1 26
#define LORA_NRST 23
#define LORA_BUSY 32

static Esp32s3Hal hal(LORA_HOST, LORA_SCK, LORA_MISO, LORA_MOSI);
static Module mod(&hal, LORA_CS, LORA_DIO1, LORA_NRST, LORA_BUSY);
static SX1262 _lora_radio(&mod);
static LoRa::RX_CB _lora_rx_callback = nullptr;
static TaskHandle_t _lora_rx_task_handle = nullptr;

static void _lora_rx_task(void *arg) {
  (void)arg;
  static uint8_t buf[256]; // max LoRa packet size

  while (true) {
    if (_lora_radio.available()) {
      int len = _lora_radio.getPacketLength();

      if (len > 0) {
        if (_lora_radio.readData(buf, len) == RADIOLIB_ERR_NONE) {
          LoRa::Packet pkt;
          pkt.data = buf;
          pkt.size = len;

          if (_lora_rx_callback) {
            _lora_rx_callback(pkt);
          }
        }
      }

      _lora_radio.startReceive(); // restart RX
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

bool LoRa::init() {
  hal.spiBegin();

  int state = _lora_radio.begin(868.0); // 868.0 MHz (EU standard)

  if (state != RADIOLIB_ERR_NONE) return false;

  if (_lora_radio.setDio2AsRfSwitch(true) != RADIOLIB_ERR_NONE) return false;
  if (_lora_radio.setBandwidth(125.0) != RADIOLIB_ERR_NONE) return false;
  if (_lora_radio.setSpreadingFactor(7) != RADIOLIB_ERR_NONE) return false;
  if (_lora_radio.setCodingRate(5) != RADIOLIB_ERR_NONE) return false;

  return true;
}

void LoRa::deinit() {
  _lora_rx_callback = nullptr;
  if (_lora_rx_task_handle) {
    vTaskDelete(_lora_rx_task_handle);
    _lora_rx_task_handle = nullptr;
  }
  _lora_radio.sleep();
}

bool LoRa::is_ready() { return _lora_radio.standby() == RADIOLIB_ERR_NONE; }

bool LoRa::tx(const Packet &data) {
  int state = _lora_radio.transmit((uint8_t *)data.data, data.size);
  return (state == RADIOLIB_ERR_NONE);
}

void LoRa::rx(RX_CB cb) {
  _lora_rx_callback = cb;

  _lora_radio.startReceive();

  if (!_lora_rx_task_handle) {
    xTaskCreate(_lora_rx_task, "LoRaRxTask", 2048, nullptr, 5, &_lora_rx_task_handle);
  }
}