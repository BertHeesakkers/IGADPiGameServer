cmake_minimum_required(VERSION 3.7.2)

project(Network)

include_directories(${ROOT_INCLUDE_FOLDER}Network)
include_directories(${ROOT_INCLUDE_FOLDER}Network/Helpers)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)

#header files
set(Network_CORE_HEADERS
	../../Include/Network/CheckConnectionAttemptResult.h
	../../Include/Network/CheckStartupResult.h
	../../Include/Network/ClientID.h
	../../Include/Network/EMessages.h
	../../Include/Network/EPlayer.h
	../../Include/Network/EServerErrors.h
	../../Include/Network/GameHelpers.h
	../../Include/Network/GameID.h
	../../Include/Network/GenerateClientID.h
	../../Include/Network/GenerateGameID.h
	../../Include/Network/MessageHelpers.h
	../../Include/Network/PacketHelpers.h
	../../Include/Network/RakNetIncludes.h
	../../Include/Network/WriteBitStream.h)

source_group("Header Files" FILES ${Network_CORE_HEADERS})

set(Network_HELPERS_HEADERS
	../../Include/Network/Helpers/Translate.h)

source_group("Header Files\\Helpers" FILES ${Network_HELPERS_HEADERS})

set(Network_AGGREGATE_HEADERS
	${Network_CORE_HEADERS}
	${Network_HELPERS_HEADERS})

# source files
set(Network_CORE_SOURCES
	../../Source/Network/CheckConnectionAttemptResult.cpp
	../../Source/Network/CheckStartupResult.cpp
	../../Source/Network/GameHelpers.cpp
	../../Source/Network/GenerateClientID.cpp
	../../Source/Network/GenerateGameID.cpp
	../../Source/Network/MessageHelpers.cpp
	../../Source/Network/PacketHelpers.cpp
	../../Source/Network/WriteBitStream.cpp)

source_group("Source Files" FILES ${Network_CORE_SOURCES})

set(Network_HELPERS_SOURCES
	../../Source/Network/Helpers/Translate.cpp)

source_group("Source Files\\Helpers" FILES ${Network_HELPERS_SOURCES})

set(Network_AGGREGATE_SOURCES
	${Network_CORE_SOURCES}
	${Network_HELPERS_SOURCES})

# setup target
add_library(Network STATIC ${Network_AGGREGATE_HEADERS} ${Network_AGGREGATE_SOURCES})

IF(WIN32)
	target_compile_definitions(Framework PUBLIC ENVIRONMENT_WINDOWS)
ELSE(WIN32)
	target_compile_definitions(Framework PUBLIC ENVIRONMENT_PI)
ENDIF(WIN32)

set_target_properties(Network PROPERTIES FOLDER "Base")

install(TARGETS Network DESTINATION lib)
