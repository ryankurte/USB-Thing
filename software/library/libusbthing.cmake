#Add usbthing driver sources
set(USBTHING_SOURCES 
	${CMAKE_CURRENT_LIST_DIR}/source/usbthing.c
	)

#Add required inclusions
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

#Create usbthing driver library
add_library(usbthing-driver ${USBTHING_SOURCES})

#Add to convenience variable
set(LIBS ${LIBS} usbthing-driver)