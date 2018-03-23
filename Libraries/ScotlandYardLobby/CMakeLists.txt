cmake_minimum_required(VERSION 3.9.2)

project(ScotlandYardLobby)

include_directories(${ROOT_INCLUDE_FOLDER}ScotlandYardLobby)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)
include_directories(${THIRD_PARTY_FOLDER}RakNet/include)

#header files
set(ScotlandYardLobby_CORE_HEADERS
	../../Include/ScotlandYardLobby/EMessages.h
	../../Include/ScotlandYardLobby/EMessagesExt.h
	../../Include/ScotlandYardLobby/ScotlandYardLobby.h
	../../Include/ScotlandYardLobby/ScotlandYardServerGame.h)

source_group("Header Files" FILES ${ScotlandYardLobby_CORE_HEADERS})

set(ScotlandYardLobby_AGGREGATE_HEADERS
	${ScotlandYardLobby_CORE_HEADERS})

# source files
set(ScotlandYardLobby_CORE_SOURCES
	../../Source/ScotlandYardLobby/ScotlandYardLobby.cpp
	../../Source/ScotlandYardLobby/ScotlandYardServerGame.cpp)

source_group("Source Files" FILES ${ScotlandYardLobby_CORE_SOURCES})

set(ScotlandYardLobby_AGGREGATE_SOURCES
	${ScotlandYardLobby_CORE_SOURCES})

# setup target
add_library(ScotlandYardLobby STATIC ${ScotlandYardLobby_AGGREGATE_HEADERS} ${ScotlandYardLobby_AGGREGATE_SOURCES})

set_target_properties(ScotlandYardLobby PROPERTIES FOLDER "ServerCode")

install(TARGETS ScotlandYardLobby DESTINATION lib EXCLUDE_FROM_ALL)
