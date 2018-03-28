cmake_minimum_required(VERSION 3.9.2)

project(BattleShipsLobby)

include_directories(${ROOT_INCLUDE_FOLDER}BattleShipsLobby)

#external dependencies
message(STATUS "Boost directory set at ${THIRD_PARTY_FOLDER}boost_1_65_1")
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)
include_directories(${THIRD_PARTY_FOLDER}RakNet/include)

#header files
set(BattleShipsLobby_CORE_HEADERS
	../../Include/BattleShipsLobby/BattleShipsPacketHelpers.h
	../../Include/BattleShipsLobby/BattleShipsServerGame.h
	../../Include/BattleShipsLobby/EMessages.h
	../../Include/BattleShipsLobby/EMessagesExt.h
	../../Include/BattleShipsLobby/MessageStructures.h
	../../Include/BattleShipsLobby/BattleShipsLobby.h)

source_group("Header Files" FILES ${BattleShipsLobby_CORE_HEADERS})

set(BattleShipsLobby_AGGREGATE_HEADERS
	${BattleShipsLobby_CORE_HEADERS})

#source files
set(BattleShipsLobby_CORE_SOURCES
	../../Source/BattleShipsLobby/BattleShipsPacketHelpers.cpp
	../../Source/BattleShipsLobby/BattleShipsServerGame.cpp
	../../Source/BattleShipsLobby/BattleShipsLobby.cpp)

source_group("Source Files" FILES ${BattleShipsLobby_CORE_SOURCES})

# setup target
add_library(BattleShipsLobby STATIC ${BattleShipsLobby_AGGREGATE_HEADERS} ${BattleShipsLobby_CORE_SOURCES})

set_target_properties(BattleShipsLobby PROPERTIES FOLDER "ServerCode")

install(TARGETS BattleShipsLobby DESTINATION lib EXCLUDE_FROM_ALL)
