cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)
#set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/pico-sdk)

set(PICO_SDK_FETCH_FROM_GIT ON)
set(PICO_SDK_FETCH_FROM_GIT_PATH ${CMAKE_CURRENT_LIST_DIR}/pico-sdk)
set(PICO_SDK_FETCH_FROM_GIT_TAG 2.1.0)

include(cmake/pico_sdk_import.cmake)

project(boop C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


pico_sdk_init()

add_subdirectory(src)
