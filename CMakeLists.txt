cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD  weact_studio_rp2040_16mb )
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

include(pico_extras_import_optional.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)


if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
#        -DDBG=1
        )

add_executable(main
        main.c
        msc_disk.c
        usb_descriptors.c
        lfs.c
        pico_hal.c
        memfind.c
        )

target_include_directories(main PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# pull in common dependencies
target_link_libraries(main  pico_stdlib hardware_adc hardware_pwm hardware_flash tinyusb_device  tinyusb_board )
#enable usb output, disable uart output
#    pico_enable_stdio_usb(main 1)
    pico_enable_stdio_uart(main 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(main)

# add url via pico_set_program_url
#example_auto_set_url(...)

