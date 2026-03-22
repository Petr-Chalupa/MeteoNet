set(src_dir "./src")
set(include_dir "./include")

file(GLOB src_files "src/*.cpp")

idf_component_register(
    SRCS ${src_files}
    INCLUDE_DIRS ${include_dir}
    REQUIRES driver esp_wifi nvs_flash esp_event
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