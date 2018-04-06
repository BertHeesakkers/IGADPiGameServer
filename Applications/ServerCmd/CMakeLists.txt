cmake_minimum_required(VERSION 3.9.2)

project(ServerCmd LANGUAGES CXX)

include_directories(${ROOT_INCLUDE_FOLDER}ServerCmd)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)
include_directories(${THIRD_PARTY_FOLDER}RakNet/include)

#header files
set(ServerCmd_CORE_HEADERS)

source_group("Header Files" FILES ${ServerCmd_CORE_HEADERS})

set(ServerCmd_AGGREGATE_HEADERS
	${ServerCmd_CORE_HEADERS})

#source files
set(ServerCmd_CORE_SOURCE
	../../Source/ServerCmd/main.cpp)

source_group("Source Files" FILES ${ServerCmd_CORE_SOURCE})

set(ServerCmd_AGGREGATE_SOURCES
	${ServerCmd_CORE_SOURCE})

#setup target
add_executable(ServerCmd ${ServerCmd_CORE_HEADERS} ${ServerCmd_CORE_SOURCE})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "Setting up linking to Windows specific libraries for ServerCmd")
	target_link_libraries(ServerCmd Framework Network)
	target_link_libraries(ServerCmd Ws2_32)
	target_link_libraries(ServerCmd debug ${RAKNET_STATIC_LIB_DEBUG})
	target_link_libraries(ServerCmd optimized ${RAKNET_STATIC_LIB_RELEASE})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message(STATUS "Setting up linking to Linux specific libraries for ServerCmd")
	target_link_libraries(ServerCmd debug -Wl,--whole-archive Framework Network ${CMAKE_THREAD_LIBS_INIT} ${RAKNET_STATIC_LIB_DEBUG} -Wl,--no-whole-archive)
	target_link_libraries(ServerCmd optimized -Wl,--whole-archive Framework Network ${CMAKE_THREAD_LIBS_INIT} ${RAKNET_STATIC_LIB_RELEASE} -Wl,--no-whole-archive)
endif()

set_target_properties(ServerCmd PROPERTIES FOLDER "Applications")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set_target_properties(ServerCmd PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Bin
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Bin)

	install(TARGETS ServerCmd DESTINATION ${CMAKE_SOURCE_DIR}/Bin)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	install(TARGETS ServerCmd DESTINATION ${CMAKE_SOURCE_DIR}/Bin)
endif()
