# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)

set(TARGET "usb-thing")

# Configure project and languages
project(${TARGET} C CXX)

# Add project headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add project sources
set(SOURCES
	source/main.c
)

##### Outputs #####

# Generate executable and link
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${LIBS})

