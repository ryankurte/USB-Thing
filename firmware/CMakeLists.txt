# Base EFM32 CMake file
#
# This can be used as is as a project base, or by adding the efm32-base
# repository as a submodule in another project, copying this CMakeLists file
# to the top level directory, and updating the toolchain paths to reflect this
# change

###### Project Environment #####

# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)

set(BASE_LOC ${CMAKE_CURRENT_LIST_DIR}/../efm32-base)

# Set the toolchain/languages/support files etc
include(${BASE_LOC}/toolchain/arm-gcc.cmake)

##### Project Setup #####

# Set our output target
set(TARGET usb-thing)

# Configure project and languages
project(${TARGET} C CXX ASM)

set(CMAKE_C_FLAGS "-std=gnu11 -g")
set(CMAKE_CXX_FLAGS "-std=gnu++11 -g")

# Set device
set(DEVICE EFM32GG332F1024)

# Set build
set(CMAKE_BUILD_TYPE DEBUG)

if(WITH_BOOTLOADER)
set(FLASH_START 0x8000)
endif(WITH_BOOTLOADER)

##### Modules #####

# Libraries should add themselves to the LIBS variable
include(${PROJECT_SOURCE_DIR}/../common/common.cmake)

# Add base libs (emlib, CMSIS, device files)
include(${BASE_LOC}/drivers/usb/usb.cmake)
include(${BASE_LOC}/toolchain/efm32-base.cmake)

##### Files #####

# Add project headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add project sources
set(SOURCES
	source/main.c
	source/handlers.c
	source/services/base_svc.c
	source/services/adc_svc.c
	source/services/dac_svc.c
	source/services/gpio_svc.c
	source/services/spi_svc.c
	source/services/i2c_svc.c
	source/peripherals/gpio.c
	source/peripherals/spi.c
	source/peripherals/i2c.c
	source/peripherals/pwm.c
	source/peripherals/adc.c
	source/peripherals/dac.c
	${EFM_USB_SOURCES}
)

##### Pre Build #####
add_custom_target(version ${PROJECT_SOURCE_DIR}/../version.py ${CMAKE_BINARY_DIR}/version.h)
include_directories(${CMAKE_BINARY_DIR})

##### Outputs #####

# Generate executable and link
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${LIBS})
add_dependencies(${TARGET} version)

##### Post build #####

# Add post build commands
include(${BASE_LOC}/toolchain/post-build.cmake)

# Add JLink commands
include(${BASE_LOC}/toolchain/jlink.cmake)
