cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Firmware C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Firmware 
    src/Firmware.c
    src/hardware/LCD.c
    src/drivers/spi_driver.c
    src/hardware/imu.c
    src/hardware/ds1302.c
    src/drivers/i2c_driver.c
    src/drivers/spi_driver.c
    src/drivers/twire_driver.c
    src/step_counter.c

)

pico_set_program_name(Firmware "Firmware")
pico_set_program_version(Firmware "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Firmware 0)
pico_enable_stdio_usb(Firmware 1)

# Add the standard include files to the build
target_include_directories(Firmware PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Firmware 
        pico_stdlib
        hardware_spi
        hardware_i2c
        hardware_pwm
        hardware_dma
        hardware_pll
        hardware_clocks
        hardware_sync
        hardware_dma  
        )
        
pico_add_extra_outputs(Firmware)