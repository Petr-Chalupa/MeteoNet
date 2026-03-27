idf_component_register(
    SRCS "BME280.cpp" "DS18B20.cpp"
    INCLUDE_DIRS "include/"
    REQUIRES esp_driver_i2c bme280 onewire_bus ds18b20
)