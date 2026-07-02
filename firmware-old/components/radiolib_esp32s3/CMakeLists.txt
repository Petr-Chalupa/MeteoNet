idf_component_register(
    SRCS
        esp32s3HAL.cpp

    INCLUDE_DIRS
        .

    REQUIRES
        jgromes__radiolib
        esp_driver_gpio
        esp_driver_spi
        esp_timer
        freertos
)

# Needed to compile RadioLib for ESP-IDF
idf_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)
