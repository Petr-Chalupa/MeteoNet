#pragma once

#include <stddef.h>
#include <stdint.h>

namespace LoRa {

struct Packet {
  const uint8_t *data;
  size_t size;
};

using RX_CB = void (*)(Packet);

/**
 * Initializes the LoRa module.
 */
void init();

/**
 * Deinitializes the LoRa module.
 */
void deinit();

/**
 * Checks if the LoRa module is ready.
 * @return true  If the LoRa module is initialized and ready
 * @return false Otherwise
 */
[[nodiscard]] bool is_ready();

/**
 * Sends packet via LoRa
 * @param data  The data to send
 * @param flush Whether to send whole TX data buffer immediately
 * @return true  If the packet was successfully put into TX buffer
 * @return false Otherwise
 */
[[nodiscard]] bool tx(const Packet &data, bool flush);

/**
 * Registers callback for data RX
 * @param cb What function to use
 */
void rx(RX_CB cb);

} // namespace LoRa