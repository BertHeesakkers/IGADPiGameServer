cmake_minimum_required(VERSION 3.7.2)

project(BattleShips)

include_directories(${ROOT_INCLUDE_FOLDER}BattleShips)

#header files
set(BattleShips_CORE_HEADERS
	../../Include/ScotlandYard/ECollision.h
	../../Include/ScotlandYard/EShip.h)

source_group("Header Files" FILES ${BattleShips_CORE_HEADERS})

set(BattleShips_AGGREGATE_HEADERS
	${BattleShips_CORE_HEADERS})

# setup target
add_library(BattleShips STATIC ${BattleShips_AGGREGATE_HEADERS})

set_target_properties(BattleShips PROPERTIES FOLDER "Games")

install(TARGETS BattleShips DESTINATION lib)
