# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)

# Configure project and languages
project(libusbthing C CXX)

# Add project headers
# Add project headers
include_directories(${PROJECT_SOURCE_DIR}/../common/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(USBTHING_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/source/usbthing.c)

# Add libraries
add_library(usbthing SHARED ${USBTHING_SOURCES})
target_link_libraries(usbthing usb-1.0)

install(TARGETS usbthing LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/usbthing)

