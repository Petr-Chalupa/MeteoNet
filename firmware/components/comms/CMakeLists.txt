idf_component_register(
    SRCS "wifi.cpp" "lora.cpp"
    INCLUDE_DIRS "include/"
    REQUIRES esp_wifi esp_event esp_netif nvs_flash
)