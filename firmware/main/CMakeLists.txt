idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "include/"
    REQUIRES comms sensors freertos
)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -fno-exceptions
    -fno-rtti
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE
    $<$<STREQUAL:${DEVICE_ROLE},ROOT>:IS_ROOT>
    $<$<STREQUAL:${DEVICE_ROLE},NODE>:IS_NODE>
)