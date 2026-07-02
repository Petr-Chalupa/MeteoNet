cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(MeteoNet)

set(DEVICE_ROLE "NODE" CACHE STRING "Device role: NODE or ROOT")