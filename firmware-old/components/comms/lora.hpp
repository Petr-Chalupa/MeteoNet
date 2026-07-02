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
 * @return true  If the stack was initialized
 * @return false Otherwise
 */
[[nodiscard]] bool init();

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
 * @param data Packet to send
 * @return true  If sent successfully
 * @return false Otherwise
 */
[[nodiscard]] bool tx(const Packet &data);

/**
 * Registers callback for data RX
 * @param cb What function to use
 */
void rx(RX_CB cb);

} // namespace LoRa