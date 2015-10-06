# - Adds library to OPTIONAL_LIBS
# - Adds headers to include directories
   
# Find headers    
find_path(USB_THING_INCLUDE_DIR
    NAMES
    usbthing.h
    PATHS
    /usr/include
    /usr/local/include
    /opt/local/include
    /sw/include
    PATH_SUFFIXES
    cp2130
)
    
# Find library
find_library(USBTHING_LIBRARY
    NAMES
    usbthing
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
)

# Check if we found anything
if (USB_THING_INCLUDE_DIR AND USBTHING_LIBRARY)
    message(STATUS "Found usbthing:")
    message(STATUS " - Include: ${USB_THING_INCLUDE_DIR}")
    message(STATUS " - Library: ${USBTHING_LIBRARY}")
else ()
    if (USBTHING_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find usbthing")
    endif (USBTHING_FIND_REQUIRED)
endif ()

# Add include directories
include_directories(${USBTHING_INCLUDE_DIR})
    
# Add library for linking
set(OPTIONAL_LIBS ${OPTIONAL_LIBS} ${USBTHING_LIBRARY})
