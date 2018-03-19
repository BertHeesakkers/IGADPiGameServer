cmake_minimum_required(VERSION 3.7.2)
project (IGADPiGameServer)

set(THIRD_PARTY_FOLDER ${CMAKE_SOURCE_DIR}/ThirdParty/)
set(ROOT_INCLUDE_FOLDER ${CMAKE_SOURCE_DIR}/Include/)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables

set(PROJECT_USER_TEMPLATE_FILE ${CMAKE_SOURCE_DIR}/Scripts/VSProjectTemplateUserFile.in)

set(PROTO_ASSET_DIR ${CMAKE_SOURCE_DIR}/Data)

set(CMAKE_DEBUG_POSTFIX "_d")

add_definitions(-DPROTO_PLATFORM_WIN32)
add_definitions(-DGFX_API_DX11)
add_definitions(-DUNICODE)

include_directories(${ROOT_INCLUDE_FOLDER})

add_subdirectory(Libraries)
add_subdirectory(Applications)
