idf_component_register(
    SRCS 
        BME280.cpp
        DS18B20.cpp

    INCLUDE_DIRS 
        .

    REQUIRES 
        esp_driver_i2c 
        onewire_bus
        bme280 
        ds18b20
)