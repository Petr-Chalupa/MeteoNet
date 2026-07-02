idf_component_register(
    SRCS 
        wifi.cpp
        lora.cpp

    INCLUDE_DIRS
        .

    REQUIRES 
        radiolib_esp32s3
        esp_wifi 
        esp_event 
        esp_netif 
        nvs_flash 
        freertos
)