# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)

set(TARGET "usb-thing")

# Configure project and languages
project(${TARGET} C CXX)

# Add project headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add libraries
include(${PROJECT_SOURCE_DIR}/../library/libusbthing.cmake)

# Add project sources
set(SOURCES
	source/main.c
)

# Pre Build
# Version header generation
add_custom_target(version ${PROJECT_SOURCE_DIR}/../version.py ${CMAKE_BINARY_DIR}/version.h)
include_directories(${CMAKE_BINARY_DIR})

# Outputs
# Generate executable and link
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${LIBS})

