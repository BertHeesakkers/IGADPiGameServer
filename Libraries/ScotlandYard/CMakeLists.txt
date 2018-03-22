cmake_minimum_required(VERSION 3.9.2)

project(ScotlandYard)

include_directories(${ROOT_INCLUDE_FOLDER}ScotlandYard)
include_directories(${ROOT_INCLUDE_FOLDER}ScotlandYard/Helpers)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)

#header files
set(ScotlandYard_CORE_HEADERS
	../../Include/ScotlandYard/Detective.h
	../../Include/ScotlandYard/Edge.h
	../../Include/ScotlandYard/ETravelOption.h
	../../Include/ScotlandYard/ETravelResult.h
	../../Include/ScotlandYard/Map.h
	../../Include/ScotlandYard/Node.h
	../../Include/ScotlandYard/Player.h
	../../Include/ScotlandYard/Ply.h
	../../Include/ScotlandYard/ScotlandYardAI.h
	../../Include/ScotlandYard/ScotlandYardGame.h
	../../Include/ScotlandYard/Spy.h
	../../Include/ScotlandYard/TravelLog.h
	../../Include/ScotlandYard/Helpers/TurnHelpers.h
	../../Include/ScotlandYard/Helpers/ValidateTravelLog.h)

source_group("Header Files" FILES ${ScotlandYard_CORE_HEADERS})

set(ScotlandYard_HELPERS_HEADERS
	../../Include/ScotlandYard/Helpers/TurnHelpers.h
	../../Include/ScotlandYard/Helpers/ValidateTravelLog.h)

source_group("Header Files\\Helpers" FILES ${ScotlandYard_HELPERS_HEADERS})

set(ScotlandYard_AGGREGATE_HEADERS
	${ScotlandYard_CORE_HEADERS}
	${ScotlandYard_HELPERS_HEADERS})

# source files
set(ScotlandYard_CORE_SOURCES
	../../Source/ScotlandYard/Detective.cpp
	../../Source/ScotlandYard/Edge.cpp
	../../Source/ScotlandYard/Map.cpp
	../../Source/ScotlandYard/Node.cpp
	../../Source/ScotlandYard/Player.cpp
	../../Source/ScotlandYard/Ply.cpp
	../../Source/ScotlandYard/ScotlandYardAI.cpp
	../../Source/ScotlandYard/ScotlandYardGame.cpp
	../../Source/ScotlandYard/Spy.cpp
	../../Source/ScotlandYard/TravelLog.cpp)

source_group("Source Files" FILES ${ScotlandYard_CORE_SOURCES})

set(ScotlandYard_HELPERS_SOURCES
	../../Source/ScotlandYard/Helpers/TurnHelpers.cpp
	../../Source/ScotlandYard/Helpers/ValidateTravelLog.cpp)

source_group("Source Files" FILES ${ScotlandYard_HELPERS_SOURCES})

set(ScotlandYard_AGGREGATE_SOURCES
	${ScotlandYard_CORE_SOURCES}
	${ScotlandYard_HELPERS_SOURCES})

# setup target
add_library(ScotlandYard STATIC ${ScotlandYard_AGGREGATE_HEADERS} ${ScotlandYard_AGGREGATE_SOURCES})

set_target_properties(ScotlandYard PROPERTIES FOLDER "Games")

install(TARGETS ScotlandYard DESTINATION lib)
