cmake_minimum_required(VERSION 3.7.2)

project(BattleShipsLobby)

include_directories(${ROOT_INCLUDE_FOLDER}BattleShipsLobby)

#header files
set(BattleShipsLobby_CORE_HEADERS
	../../Include/BattleShipsLobby/EMessages.h)

source_group("Header Files" FILES ${BattleShipsLobby_CORE_HEADERS})

set(BattleShipsLobby_AGGREGATE_HEADERS
	${BattleShipsLobby_CORE_HEADERS})

# setup target
add_library(BattleShipsLobby STATIC ${BattleShipsLobby_AGGREGATE_HEADERS})

set_target_properties(BattleShipsLobby PROPERTIES FOLDER "Games")

install(TARGETS BattleShipsLobby DESTINATION lib)
