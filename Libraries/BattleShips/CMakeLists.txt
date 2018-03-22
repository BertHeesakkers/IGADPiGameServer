cmake_minimum_required(VERSION 3.9.2)

project(BattleShips)

include_directories(${ROOT_INCLUDE_FOLDER}BattleShips)
include_directories(${ROOT_INCLUDE_FOLDER}BattleShips/Helpers)

#header files
set(BattleShips_CORE_HEADERS
	../../Include/BattleShips/BattleShipsGame.h
	../../Include/BattleShips/BoardData.h
	../../Include/BattleShips/ECollision.h
	../../Include/BattleShips/EDirection.h
	../../Include/BattleShips/EShip.h
	../../Include/BattleShips/GetShipSize.h
	../../Include/BattleShips/ShipPlacementInfo.h
	../../Include/BattleShips/BattleShipsAI.h
	../../Include/BattleShips/BattleShipsBoard.h)

source_group("Header Files" FILES ${BattleShips_CORE_HEADERS})

set(BattleShips_HELPER_HEADERS
	../../Include/BattleShips/Helpers/GetDirection.h)

source_group("Header Files\\Helpers" FILES ${BattleShips_HELPER_HEADERS})

set(BattleShips_AGGREGATE_HEADERS
	${BattleShips_CORE_HEADERS}
	${BattleShips_HELPER_HEADERS})

#source files
set(BattleShips_CORE_SOURCES
	../../Source/BattleShips/BattleShipsAI.cpp
	../../Source/BattleShips/BattleShipsBoard.cpp
	../../Source/BattleShips/BattleShipsGame.cpp
	../../Source/BattleShips/GetShipSize.cpp
	../../Source/BattleShips/ShipPlacementInfo.cpp)

source_group("Source Files" FILES ${BattleShips_CORE_SOURCES})

set(BattleShips_HELPER_SOURCES
	../../Source/BattleShips/Helpers/GetDirection.cpp)

source_group("Source Files\\Helpers" FILES ${BattleShips_HELPER_SOURCES})

set(BattleShips_AGGREGATE_SOURCES
	${BattleShips_CORE_SOURCES}
	${BattleShips_HELPER_SOURCES})

# setup target
add_library(BattleShips STATIC ${BattleShips_AGGREGATE_HEADERS} ${BattleShips_AGGREGATE_SOURCES})

set_target_properties(BattleShips PROPERTIES FOLDER "Games")

install(TARGETS BattleShips DESTINATION lib)
