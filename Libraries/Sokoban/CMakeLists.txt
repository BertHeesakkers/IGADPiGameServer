cmake_minimum_required(VERSION 3.9.2)

project(Sokoban)

include_directories(${ROOT_INCLUDE_FOLDER}Sokoban)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)

#header files
set(Sokoban_CORE_HEADERS
	../../Include/Sokoban/EElement.h
	../../Include/Sokoban/Level.h
	../../Include/Sokoban/LevelLoader.h
	../../Include/Sokoban/OutputLevel.h
	../../Include/Sokoban/SokobanAI.h
	../../Include/Sokoban/SokobanGame.h)

source_group("Header Files" FILES ${Sokoban_CORE_HEADERS})

set(Sokoban_AGGREGATE_HEADERS
	${Sokoban_CORE_HEADERS})

# source files
set(Sokoban_CORE_SOURCES
	../../Source/Sokoban/Level.cpp
	../../Source/Sokoban/LevelLoader.cpp
	../../Source/Sokoban/OutputLevel.cpp
	../../Source/Sokoban/SokobanAI.cpp
	../../Source/Sokoban/SokobanGame.cpp)

source_group("Source Files" FILES ${Sokoban_CORE_SOURCES})

set(Sokoban_AGGREGATE_SOURCES
	${Sokoban_CORE_SOURCES})

# setup target
add_library(Sokoban STATIC ${Sokoban_AGGREGATE_HEADERS} ${Sokoban_AGGREGATE_SOURCES})

set_target_properties(Sokoban PROPERTIES FOLDER "Games")

install(TARGETS Sokoban DESTINATION lib EXCLUDE_FROM_ALL)
