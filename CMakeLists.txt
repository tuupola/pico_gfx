cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Default path to the SDK
if (NOT PICO_SDK_PATH)
  set(PICO_SDK_PATH "../pico-sdk")
endif ()

# Pull in Pico SDK (must be before project)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
project(firmware C CXX ASM)

pico_sdk_init()

add_executable(firmware main.c clock.c)

pico_set_program_name(firmware "Pico GFX")
pico_set_program_version(firmware "0.1")

# Enable usb output, disable uart output
pico_enable_stdio_usb(firmware 1)
pico_enable_stdio_uart(firmware 0)

# Add the standard library to the build
target_link_libraries(firmware pico_stdlib)

pico_add_extra_outputs(firmware)

# Add HAGL and the HAL
add_subdirectory(external/hagl)
add_subdirectory(external/hagl_hal)
target_link_libraries(firmware hagl hagl_hal)

target_compile_definitions(firmware PRIVATE
  NO_MENUCONFIG
# HAGL_HAL_USE_SINGLE_BUFFER
  HAGL_HAL_USE_DOUBLE_BUFFER
# HAGL_HAL_USE_TRIPLE_BUFFER
  HAGL_HAL_USE_DMA
  HAGL_HAL_DEBUG
)
