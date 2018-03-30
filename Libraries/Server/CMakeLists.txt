cmake_minimum_required(VERSION 3.9.2)

project(Server)

include_directories(${ROOT_INCLUDE_FOLDER}Server)
include_directories(${ROOT_INCLUDE_FOLDER}Server/Helpers)
include_directories(${ROOT_INCLUDE_FOLDER}Server/Utility)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER})
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)
include_directories(${THIRD_PARTY_FOLDER}RakNet/include)

#header files
set(Server_CORE_HEADERS
	../../Include/Server/BaseLobby.h
	../../Include/Server/BaseServerGame.h
	../../Include/Server/EGame.h
	../../Include/Server/GameData.h
	../../Include/Server/ILobby.h
	../../Include/Server/IServerGame.h
	../../Include/Server/LobbyData.h
	../../Include/Server/Server.h
	../../Include/Server/ServerParameters.h
	../../Include/Server/UserData.h)

source_group("Header Files" FILES ${Server_CORE_HEADERS})

set(Server_HELPERS_HEADERS
	../../Include/Server/Helpers/LobbyHelpers.h
	../../Include/Server/Helpers/ParseServerParameters.h
	../../Include/Server/Helpers/UserDataHelpers.h)

source_group("Header Files\\Helpers" FILES ${Server_HELPERS_HEADERS})

set(Server_UTILITY_HEADERS
	../../Include/Server/Utility/ReadUserData.h
	../../Include/Server/Utility/WriteUserData.h)

source_group("Header Files\\Utility" FILES ${Server_UTILITY_HEADERS})

set(Server_AGGREGATE_HEADERS
	${Server_CORE_HEADERS}
	${Server_HELPERS_HEADERS}
	${Server_UTILITY_HEADERS})

# source files
set(Server_CORE_SOURCES
	../../Source/Server/BaseLobby.cpp
	../../Source/Server/BaseServerGame.cpp
	../../Source/Server/Server.cpp
)

source_group("Source Files" FILES ${Server_CORE_SOURCES})

set(Server_HELPERS_SOURCES
	../../Source/Server/Helpers/LobbyHelpers.cpp
	../../Source/Server/Helpers/ParseServerParameters.cpp
	../../Source/Server/Helpers/UserDataHelpers.cpp)

source_group("Source Files\\Helpers" FILES ${Server_HELPERS_SOURCES})

set(Server_UTILITY_SOURCES
	../../Source/Server/Utility/ReadUserData.cpp
	../../Source/Server/Utility/WriteUserData.cpp)

source_group("Source Files" FILES ${Server_UTILITY_SOURCES})

set(Server_AGGREGATE_SOURCES
	${Server_CORE_SOURCES}
	${Server_HELPERS_SOURCES}
	${Server_UTILITY_SOURCES})

# setup target
add_definitions(-DNOMINMAX)

add_library(Server STATIC ${Server_AGGREGATE_HEADERS} ${Server_AGGREGATE_SOURCES})

set_target_properties(Server PROPERTIES FOLDER "ServerCode")

install(TARGETS Server DESTINATION lib EXCLUDE_FROM_ALL)
