cmake_minimum_required(VERSION 3.7.2)

project(Tron)

include_directories(${ROOT_INCLUDE_FOLDER}Tron)

#header files
set(Tron_CORE_HEADERS
	../../Include/Tron/Arena.h
	../../Include/Tron/Bike.h
	../../Include/Tron/TronAI.h
	../../Include/Tron/TronGame.h)

source_group("Header Files" FILES ${Tron_CORE_HEADERS})

set(Tron_AGGREGATE_HEADERS
	${Tron_CORE_HEADERS})

# source files
set(Tron_CORE_SOURCES
	../../Source/Tron/Arena.cpp
	../../Source/Tron/Bike.cpp
	../../Source/Tron/TronAI.cpp
	../../Source/Tron/TronGame.cpp)

source_group("Source Files" FILES ${Tron_CORE_SOURCES})

set(Tron_AGGREGATE_SOURCES
	${Tron_CORE_SOURCES})

# setup target
add_library(Tron STATIC ${Tron_AGGREGATE_HEADERS} ${Tron_AGGREGATE_SOURCES})

set_target_properties(Tron PROPERTIES FOLDER "Games")

install(TARGETS Tron DESTINATION lib)
