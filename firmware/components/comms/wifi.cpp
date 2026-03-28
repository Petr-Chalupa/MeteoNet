#include "wifi.hpp"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include <cstring>

bool WiFi::init() {
  esp_err_t nvs_err = nvs_flash_init();
  if (nvs_err == ESP_ERR_NVS_NO_FREE_PAGES || nvs_err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    nvs_flash_erase();
    nvs_err = nvs_flash_init();
  }
  if (nvs_err != ESP_OK) return false;

  if (esp_netif_init() != ESP_OK) return false;
  if (esp_event_loop_create_default() != ESP_OK) return false;
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  if (esp_wifi_init(&cfg) != ESP_OK) return false;

  if (esp_wifi_set_mode(WIFI_MODE_STA) != ESP_OK) return false;
  if (esp_wifi_start() != ESP_OK) return false;

  return true;
}

void WiFi::deinit() {
  esp_wifi_deinit();
  esp_event_loop_delete_default();
  esp_netif_deinit();
}

void WiFi::connect(const char *ssid, const char *pwd) {
  wifi_config_t wifi_config = {};

  strlcpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
  strlcpy((char *)wifi_config.sta.password, pwd, sizeof(wifi_config.sta.password));
  wifi_config.sta.failure_retry_cnt = 5;

  esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
  esp_wifi_connect();
}

bool WiFi::disconnect() {
  if (esp_wifi_disconnect() != ESP_OK) return false;
  if (esp_wifi_stop() != ESP_OK) return false;

  return true;
}

bool WiFi::is_connected() {
  esp_netif_ip_info_t ip_info;
  esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

  if (netif != nullptr && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
    return ip_info.ip.addr != 0;
  }

  return false;
}
