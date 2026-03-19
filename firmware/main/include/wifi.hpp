#pragma once

/**
 * Initializes the WiFi stack and attempts to connect
 * @param ssid The string containing the WiFi Network Name.
 * @param pwd  The string containing the WiFi Password.
 */
void wifi_init(char *ssid, char *pwd);

/**
 * Checks the current network connectivity status
 * @return true  If the station is connected
 * @return false Othrewise
 */
bool wifi_is_connected();
