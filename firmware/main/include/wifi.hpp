#pragma once

#include <stddef.h>
#include <stdint.h>

namespace WiFi {

struct Packet {
  const uint8_t *data;
  size_t size;
};

using RX_CB = void (*)(Packet);

/**
 * Initializes the WiFi stack and attempts to connect.
 * Connection attempt is retried 5 times.
 * @param ssid The string containing the WiFi network name
 * @param pwd  The string containing the WiFi password
 */
void init(const char *ssid, const char *pwd);

/**
 * Disconnects from WiFi network and deinitializes the WiFi stack
 */
void disconnect();

/**
 * Checks the current network connectivity status.
 * @return true  If the station is connected
 * @return false Otherwise
 */
[[nodiscard]] bool is_connected();

/**
 * Sends packet via WiFi
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

} // namespace WiFi
