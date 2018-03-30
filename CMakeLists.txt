cmake_minimum_required(VERSION 3.9.2)
project (IGADPiGameServer)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++11")
endif()

set(THIRD_PARTY_FOLDER ${CMAKE_SOURCE_DIR}/Thirdparty/)
set(ROOT_INCLUDE_FOLDER ${CMAKE_SOURCE_DIR}/Include/)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables

set(CMAKE_DEBUG_POSTFIX "_d")

include_directories(${ROOT_INCLUDE_FOLDER})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-D_WINSOCKAPI_)
endif()

add_subdirectory(Libraries)
add_subdirectory(Applications)
