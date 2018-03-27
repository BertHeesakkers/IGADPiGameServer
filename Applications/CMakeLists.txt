cmake_minimum_required(VERSION 3.7.2)
project (Applications)

message(STATUS "Locating third party libraries")
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        find_library(RAKNET_STATIC_LIB_DEBUG RakNetLibStatic.lib HINTS ${THIRD_PARTY_FOLDER}RakNet/lib/Debug)
        if(NOT RAKNET_STATIC_LIB_DEBUG)
          message(FATAL_ERROR "RakNet static debug library not found!")
        endif()

        find_library(RAKNET_STATIC_LIB_RELEASE RakNetLibStatic.lib HINTS ${THIRD_PARTY_FOLDER}RakNet/lib/Release)
        if(NOT RAKNET_STATIC_LIB_RELEASE)
          message(FATAL_ERROR "RakNet static release library not found!")
        endif()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        find_library(RAKNET_STATIC_LIB_DEBUG RakNetLibStatic)
        if(NOT RAKNET_STATIC_LIB_DEBUG)
          message(FATAL_ERROR "RakNet static debug library not found!")
        endif()

        find_library(RAKNET_STATIC_LIB_RELEASE RakNetLibStatic)
        if(NOT RAKNET_STATIC_LIB_RELEASE)
          message(FATAL_ERROR "RakNet static release library not found!")
        endif()

        find_package(Threads)
endif()

add_subdirectory(ServerApp)
add_subdirectory(ServerCmd)

if(EXISTS "${CMAKE_SOURCE_DIR}/Applications/ClientApp/CMakeLists.txt")
	add_subdirectory(ClientApp)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/Applications/BattleShipsTrainer/CMakeLists.txt")
	add_subdirectory(BattleShipsTrainer)
endif()
