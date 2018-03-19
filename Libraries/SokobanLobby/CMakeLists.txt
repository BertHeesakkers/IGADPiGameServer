cmake_minimum_required(VERSION 3.7.2)

project(SokobanLobby)

include_directories(${ROOT_INCLUDE_FOLDER}SokobanLobby)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)
include_directories(${THIRD_PARTY_FOLDER}RakNet/include)

#header files
set(SokobanLobby_CORE_HEADERS
	../../Include/SokobanLobby/EMessages.h
	../../Include/SokobanLobby/EMessagesExt.h
	../../Include/SokobanLobby/SokobanLobby.h
	../../Include/SokobanLobby/SokobanServerGame.h
)

source_group("Header Files" FILES ${SokobanLobby_CORE_HEADERS})

set(SokobanLobby_AGGREGATE_HEADERS
	${SokobanLobby_CORE_HEADERS})

# source files
set(SokobanLobby_CORE_SOURCES
	../../Source/SokobanLobby/SokobanLobby.cpp
	../../Source/SokobanLobby/SokobanServerGame.cpp)

source_group("Source Files" FILES ${SokobanLobby_CORE_SOURCES})

set(SokobanLobby_AGGREGATE_SOURCES
	${SokobanLobby_CORE_SOURCES})

# setup target
add_library(SokobanLobby STATIC ${SokobanLobby_AGGREGATE_HEADERS} ${SokobanLobby_AGGREGATE_SOURCES})

set_target_properties(SokobanLobby PROPERTIES FOLDER "ServerCode")

install(TARGETS SokobanLobby DESTINATION lib)
