cmake_minimum_required(VERSION 3.9.2)

project(ServerApp LANGUAGES CXX)

include_directories(${ROOT_INCLUDE_FOLDER}ServerApp)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)
include_directories(${THIRD_PARTY_FOLDER}RakNet/include)

#header files
set(ServerApp_CORE_HEADERS
	../../Include/ServerApp/ServerLog.h
	../../Include/ServerApp/ServerLogger.h
	../../Include/ServerApp/main.h
	../../Include/ServerApp/ServerApp.h)

source_group("Header Files" FILES ${ServerApp_CORE_HEADERS})

set(ServerApp_AGGREGATE_HEADERS
	${ServerApp_CORE_HEADERS})

#source files
set(ServerApp_CORE_SOURCE
	../../Source/ServerApp/main.cpp
	../../Source/ServerApp/ServerApp.cpp
	../../Source/ServerApp/ServerLog.cpp
	../../Source/ServerApp/ServerLogger.cpp)

source_group("Source Files" FILES ${ServerApp_CORE_SOURCE})

set(ServerApp_AGGREGATE_SOURCES
	${ServerApp_CORE_SOURCE})

#setup target
add_executable(ServerApp ${ServerApp_CORE_HEADERS} ${ServerApp_CORE_SOURCE})

find_package(Threads)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "Setting up linking to Windows specific libraries")
	target_link_libraries(ServerApp BattleShips BattleShipsLobby Framework Network ScotlandYard ScotlandYardLobby Server Sokoban SokobanLobby Tron TronLobby)
    target_link_libraries(ServerApp Ws2_32)
	
	target_link_libraries(ServerApp debug ${RAKNET_STATIC_LIB_DEBUG})
	target_link_libraries(ServerApp optimized ${RAKNET_STATIC_LIB_RELEASE})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message(STATUS "Setting up linking to Linux specific libraries")
	target_link_libraries(ServerApp debug -Wl,--whole-archive 
		BattleShips BattleShipsLobby Framework Network ScotlandYard ScotlandYardLobby Server Sokoban SokobanLobby Tron TronLobby ${CMAKE_THREAD_LIBS_INIT} ${RAKNET_STATIC_LIB_DEBUG} -Wl,--no-whole-archive)

	target_link_libraries(ServerApp release -Wl,--whole-archive 
		BattleShips BattleShipsLobby Framework Network ScotlandYard ScotlandYardLobby Server Sokoban SokobanLobby Tron TronLobby ${CMAKE_THREAD_LIBS_INIT} ${RAKNET_STATIC_LIB_RELEASE} -Wl,--no-whole-archive)

endif()


set_target_properties(ServerApp PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(ServerApp PROPERTIES FOLDER "Applications")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set_target_properties(ServerApp PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Bin
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Bin)

	install(TARGETS ServerApp DESTINATION ${CMAKE_SOURCE_DIR}/Bin)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	install(TARGETS ServerApp DESTINATION ${CMAKE_SOURCE_DIR}/Bin)
endif()



