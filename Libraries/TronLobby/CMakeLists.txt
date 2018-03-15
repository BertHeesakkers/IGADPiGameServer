cmake_minimum_required(VERSION 3.9.2)

project(TronLobby)

include_directories(${ROOT_INCLUDE_FOLDER}TronLobby)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)
include_directories(${THIRD_PARTY_FOLDER}RakNet/include)

#header files
set(TronLobby_CORE_HEADERS
	../../Include/TronLobby/EMessages.h
	../../Include/TronLobby/EMessagesExt.h
	../../Include/TronLobby/TronLobby.h
	../../Include/TronLobby/TronServerGame.h)

source_group("Header Files" FILES ${TronLobby_CORE_HEADERS})

set(TronLobby_AGGREGATE_HEADERS
	${TronLobby_CORE_HEADERS})

# source files
set(TronLobby_CORE_SOURCES
	../../Source/TronLobby/TronLobby.cpp
	../../Source/TronLobby/TronServerGame.cpp)

source_group("Source Files" FILES ${TronLobby_CORE_SOURCES})

set(TronLobby_AGGREGATE_SOURCES
	${TronLobby_CORE_SOURCES})

# setup target
add_library(TronLobby STATIC ${TronLobby_AGGREGATE_HEADERS} ${TronLobby_AGGREGATE_SOURCES})

set_target_properties(TronLobby PROPERTIES FOLDER "ServerCode")

install(TARGETS TronLobby DESTINATION lib)
