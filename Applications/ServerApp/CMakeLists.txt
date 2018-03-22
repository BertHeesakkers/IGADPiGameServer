cmake_minimum_required(VERSION 3.9.2)

project(ServerApp LANGUAGES CXX)

include_directories(${ROOT_INCLUDE_FOLDER}ServerApp)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)
include_directories(${THIRD_PARTY_FOLDER}RakNet/include)

#header files
set(ServerApp_CORE_HEADERS
	../../Include/ServerApp/ServerLog.h
	../../Include/ServerApp/ServerLogger.h
	../../Include/ServerApp/main.h
	../../Include/ServerApp/ServerApp.h)

source_group("Header Files" FILES ${ServerApp_CORE_HEADERS})

set(ServerApp_AGGREGATE_HEADERS
	${ServerApp_CORE_HEADERS})

#source files
set(ServerApp_CORE_SOURCE
	../../Source/ServerApp/main.cpp
	../../Source/ServerApp/ServerApp.cpp
	../../Source/ServerApp/ServerLog.cpp
	../../Source/ServerApp/ServerLogger.cpp)

source_group("Source Files" FILES ${ServerApp_CORE_SOURCE})

set(ServerApp_AGGREGATE_SOURCES
	${ServerApp_CORE_SOURCE})

#setup target
add_executable(ServerApp ${ServerApp_CORE_HEADERS} ${ServerApp_CORE_SOURCE})

find_library(RAKNET_STATIC_LIB_DEBUG RakNetLibStatic.lib HINTS ${THIRD_PARTY_FOLDER}RakNet/lib/Debug)
if(NOT RAKNET_STATIC_LIB_DEBUG)
  message(FATAL_ERROR "RakNet static debug library not found!")
endif()

find_library(RAKNET_STATIC_LIB_RELEASE RakNetLibStatic.lib HINTS ${THIRD_PARTY_FOLDER}RakNet/lib/Release)
if(NOT RAKNET_STATIC_LIB_RELEASE)
  message(FATAL_ERROR "RakNet static release library not found!")
endif()

target_link_libraries(ServerApp Framework Network BattleShips ScotlandYard Sokoban Tron ScotlandYardLobby Server BattleShipsLobby SokobanLobby TronLobby Ws2_32)
target_link_libraries(ServerApp debug ${RAKNET_STATIC_LIB_DEBUG})
target_link_libraries(ServerApp optimized ${RAKNET_STATIC_LIB_RELEASE})

set_target_properties(ServerApp PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(ServerApp PROPERTIES FOLDER "Applications")

install(TARGETS ServerApp DESTINATION bin)
