idf_component_register(
    SRCS "BME280.cpp" "DS18B20.cpp"
    INCLUDE_DIRS "include/"
    REQUIRES drivers i2c_master string
)