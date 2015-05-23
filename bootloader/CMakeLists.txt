# Base EFM32 CMake file
#
# This can be used as is as a project base, or by adding the efm32-base
# repository as a submodule in another project, copying this CMakeLists file
# to the top level directory, and updating the toolchain paths to reflect this
# change

###### Project Environment #####

# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)

# Check which directory we are running in so we can find our toolchains etc
get_filename_component(BASE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} NAME)
if(NOT ${BASE_DIRECTORY} STREQUAL "efm32-base")
	set(PROJECT_PREFIX "efm32-base")
endif()
message("Project prefix: " ${PROJECT_PREFIX})

# Set the toolchain/languages/support files etc
include(${CMAKE_CURRENT_LIST_DIR}/${PROJECT_PREFIX}/toolchain/arm-gcc.cmake)

##### Project Setup #####

# Set our output target
set(TARGET bootloader)

# Configure project and languages
project(${TARGET} C CXX ASM)

# Set device
set(DEVICE EFM32GG332F1024)

# Set build
set(CMAKE_BUILD_TYPE DEBUG)

##### Modules #####

# Libraries should add themselves to the LIBS variable

# Add base libs (emlib, CMSIS, device files)
include(${PROJECT_SOURCE_DIR}/efm32-base/drivers/usb/usb.cmake)
include(${PROJECT_SOURCE_DIR}/${PROJECT_PREFIX}/toolchain/efm32-base.cmake)

##### Files #####

# Add project headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add project sources
set(SOURCES
	source/main.c
	source/autobaud.c
	source/boot.c
	source/bootldio.c
	source/crc.c
	source/flash.c
	source/loader.c
	source/retargetdebug.c
	source/xmodem.c
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
include(${CMAKE_CURRENT_LIST_DIR}/${PROJECT_PREFIX}/toolchain/post-build.cmake)

# Add JLink commands
include(${CMAKE_CURRENT_LIST_DIR}/${PROJECT_PREFIX}/toolchain/jlink.cmake)