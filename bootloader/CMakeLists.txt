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
set(TARGET bootloader)

# Configure project and languages
project(${TARGET} C CXX ASM)

# Set device
set(DEVICE EFM32GG332F1024)

# Set flash start address
set(FLASH_START 0x0000000)
#set(USE_FLTO 1)

# Set build
set(CMAKE_BUILD_TYPE DEBUG)

##### Modules #####

# Libraries should add themselves to the LIBS variable

# Add base libs (emlib, CMSIS, device files)
include(${BASE_LOC}/drivers/usb/usb.cmake)
include(${BASE_LOC}/toolchain/efm32-base.cmake)

##### Files #####

# Add project headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add project sources
set(MAIN_SOURCES
	source/main.c
	source/autobaud.c
	source/boot.c
	source/bootldio.c
	source/crc.c
	source/flash.c
	source/retargetdebug.c
	source/xmodem.c
	source/cdc.c
	${EFM_USB_SOURCES}
)

set(SHIM_SOURCES
	source/loader.c
	source/boot.c
)

##### Pre Build #####
add_custom_target(version ${PROJECT_SOURCE_DIR}/../version.py ${CMAKE_BINARY_DIR}/version.h)
include_directories(${CMAKE_BINARY_DIR})

##### Outputs #####

# Generate executable and link
add_executable(${TARGET} ${MAIN_SOURCES})
target_link_libraries(${TARGET} ${LIBS})
add_dependencies(${TARGET} version)

##### Post build #####

# Add post build commands
include(${BASE_LOC}/toolchain/post-build.cmake)

# Add JLink commands
include(${BASE_LOC}/toolchain/jlink.cmake)

# TODO: combine binaries

