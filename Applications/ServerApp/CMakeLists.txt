cmake_minimum_required(VERSION 3.7.2)

project(ServerApp LANGUAGES CXX)

include_directories(${ROOT_INCLUDE_FOLDER}ServerApp)
include_directories(${ROOT_INCLUDE_FOLDER}RakNet)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)

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

target_link_libraries(ServerApp Framework Network RakNet ScotlandYard Sokoban Tron ScotlandYardLobby Server SokobanLobby TronLobby Ws2_32)

set_target_properties(ServerApp PROPERTIES FOLDER "Applications")

install(TARGETS ServerApp DESTINATION bin)
