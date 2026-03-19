set(src_dir "./src")
set(include_dir "./include")

file(GLOB src_files "src/*.cpp")

idf_component_register(
    SRCS "${src_files}"
    INCLUDE_DIRS "${include_dir}"
    REQUIRES driver esp_wifi nvs_flash esp_event
)

target_compile_options(${COMPONENT_LIB} PRIVATE
    "-Wall"
    "-Wextra" 
    "-Wpedantic"
    "-fno-exceptions" 
    "-fno-rtti"
    "-O2" 
)

if(NOT DEFINED DEVICE_ROLE)
    set(DEVICE_ROLE "NODE")
endif()

if(DEVICE_ROLE STREQUAL "ROOT")
    target_compile_definitions(${COMPONENT_LIB} PRIVATE IS_ROOT_NODE)
endif()