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
 * Initializes the WiFi stack
 * @return true  If the stack was initialized
 * @return false Otherwise
 */
[[nodiscard]] bool init();

/**
 * Deinitializes the WiFi stack.
 * Prepares for deep sleep or complete reset.
 */
void deinit();

/**
 * Attempts to connect to WiFi network 5 times.
 * @param ssid The string containing the WiFi network name
 * @param pwd  The string containing the WiFi password
 */
void connect(const char *ssid, const char *pwd);

/**
 * Disconnects from WiFi network
 * @return true  If the disconnection was successful
 * @return false Otherwise
 */
[[nodiscard]] bool disconnect();

/**
 * Checks the current network connectivity status.
 * @return true  If the station is connected
 * @return false Otherwise
 */
[[nodiscard]] bool is_connected();

/**
 * Sends packet via WiFi
 * @param data  The data to send
 * @return true  If the packet was successfully sent
 * @return false Otherwise
 */
[[nodiscard]] bool tx(const Packet &data);

/**
 * Registers callback for data RX
 * @param cb What function to use
 */
void rx(RX_CB cb);

} // namespace WiFi
