cmake_minimum_required(VERSION 3.9.2)

project(BattleShipsTrainer LANGUAGES CXX)

include_directories(${ROOT_INCLUDE_FOLDER}BattleShipsTrainer)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)
include_directories(${THIRD_PARTY_FOLDER}RakNet/include)

#header files
set(BattleShipsTrainer_CORE_HEADERS
	../../Include/BattleShipsTrainer/main.h)

source_group("Header Files" FILES ${BattleShipsTrainer_CORE_HEADERS})

set(BattleShipsTrainer_AGGREGATE_HEADERS
	${BattleShipsTrainer_CORE_HEADERS})

#source files
set(BattleShipsTrainer_CORE_SOURCE
	../../Source/BattleShipsTrainer/main.cpp)

source_group("Source Files" FILES ${BattleShipsTrainer_CORE_SOURCE})

set(BattleShipsTrainer_AGGREGATE_SOURCES
	${BattleShipsTrainer_CORE_SOURCE})

#setup target
add_executable(BattleShipsTrainer ${BattleShipsTrainer_CORE_HEADERS} ${BattleShipsTrainer_CORE_SOURCE})

target_link_libraries(BattleShipsTrainer Framework Network RakNet ScotlandYard Sokoban Tron ScotlandYardLobby Server SokobanLobby TronLobby Ws2_32)

set_target_properties(BattleShipsTrainer PROPERTIES FOLDER "Applications")

install(TARGETS BattleShipsTrainer DESTINATION bin)
