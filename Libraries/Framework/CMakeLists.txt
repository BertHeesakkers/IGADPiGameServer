cmake_minimum_required(VERSION 3.7.2)

project(Framework)

add_definitions(-DENVIRONMENT_WINDOWS)

include_directories(${ROOT_INCLUDE_FOLDER}Framework)
include_directories(${ROOT_INCLUDE_FOLDER}Framework/IPC)
include_directories(${ROOT_INCLUDE_FOLDER}Framework/Logging)
include_directories(${ROOT_INCLUDE_FOLDER}Framework/Math)
include_directories(${ROOT_INCLUDE_FOLDER}Framework/Math/RNG)

#external dependencies
include_directories(${THIRD_PARTY_FOLDER}boost_1_65_1)

#header files
set(Framework_CORE_HEADERS
	../../Include/Framework/AssertMessage.h
	../../Include/Framework/AutoPointer.h
	../../Include/Framework/ClearConsoleWindow.h
	../../Include/Framework/ClearContainerContents.h
	../../Include/Framework/CommandLine.h
	../../Include/Framework/CommandLineParser.h
	../../Include/Framework/ConsoleProcedure.h
	../../Include/Framework/DateTimeHelpers.h
	../../Include/Framework/FileHelpers.h
	../../Include/Framework/GetLastErrorAsString.h
	../../Include/Framework/HashedString.h
	../../Include/Framework/HashFunctions.h
	../../Include/Framework/LexicalCast.h
	../../Include/Framework/NoCopy.h
	../../Include/Framework/ResizeConsoleWindow.h
	../../Include/Framework/Sleep.h
	../../Include/Framework/Stopwatch.h
	../../Include/Framework/StringFunctions.h)

source_group("Header Files" FILES ${Framework_CORE_HEADERS})

set(Framework_IPC_HEADERS
	../../Include/Framework/IPC/AnonymousPipe.h
	../../Include/Framework/IPC/NamedPipe.h
	../../Include/Framework/IPC/Pipe.h
	../../Include/Framework/IPC/PipeClient.h
	../../Include/Framework/IPC/PipeConstants.h
	../../Include/Framework/IPC/PipeCreateParameters.h
	../../Include/Framework/IPC/PipeServer.h
	../../Include/Framework/IPC/WriteToNamedPipe.h)

source_group("Header Files\\IPC" FILES ${Framework_IPC_HEADERS})

set(Framework_LOGGING_HEADERS
	../../Include/Framework/Logging/ConsoleLogger.h
	../../Include/Framework/Logging/FileLogger.h
	../../Include/Framework/Logging/ILogger.h)

source_group("Header Files\\Logging" FILES ${Framework_LOGGING_HEADERS})

set(Framework_MATH_HEADERS
	../../Include/Framework/Math/Min.h)

source_group("Header Files\\Math" FILES ${Framework_MATH_HEADERS})

set(Framework_MATH_RNG_HEADERS
	../../Include/Framework/Math/RNG/CreateRandomNumberGenerator.h
	../../Include/Framework/Math/RNG/IRandomNumberGenerator.h
	../../Include/Framework/Math/RNG/MersenneTwister.h
	../../Include/Framework/Math/RNG/mtrand.h)

source_group("Header Files\\Math\\RNG" FILES ${Framework_MATH_RNG_HEADERS})

set(Framework_AGGREGATE_HEADERS
	${Framework_CORE_HEADERS}
	${Framework_IPC_HEADERS}
	${Framework_LOGGING_HEADERS}
	${Framework_MATH_HEADERS}
	${Framework_MATH_RNG_HEADERS})

# source files
set(Framework_CORE_SOURCES
	../../Source/Framework/AssertMessage.cpp
	../../Source/Framework/ClearConsoleWindow.cpp
	../../Source/Framework/CommandLine.cpp
	../../Source/Framework/CommandLineParser.cpp
	../../Source/Framework/ConsoleProcedure.cpp
	../../Source/Framework/DateTimeHelpers.cpp
	../../Source/Framework/FileHelpers.cpp
	../../Source/Framework/GetLastErrorAsString.cpp
	../../Source/Framework/HashedString.cpp
	../../Source/Framework/ResizeConsoleWindow.cpp
	../../Source/Framework/Sleep.cpp
	../../Source/Framework/Stopwatch.cpp
	../../Source/Framework/StringFunctions.cpp)

source_group("Source Files" FILES ${Framework_CORE_SOURCES})

set(Framework_IPC_SOURCES
	../../Source/Framework/IPC/AnonymousPipe.cpp
	../../Source/Framework/IPC/NamedPipe.cpp
	../../Source/Framework/IPC/Pipe.cpp
	../../Source/Framework/IPC/PipeClient.cpp
	../../Source/Framework/IPC/PipeServer.cpp
	../../Source/Framework/IPC/WriteToNamedPipe.cpp)

source_group("Source Files\\IPC" FILES ${Framework_IPC_SOURCES})

set(Framework_LOGGING_SOURCES
	../../Source/Framework/Logging/ConsoleLogger.cpp
	../../Source/Framework/Logging/FileLogger.cpp)

source_group("Source Files\\Logging" FILES ${Framework_LOGGING_SOURCES})

set(Framework_MATH_RNG_SOURCES
	../../Source/Framework/Math/RNG/CreateRandomNumberGenerator.cpp
	../../Source/Framework/Math/RNG/MersenneTwister.cpp
	../../Source/Framework/Math/RNG/mtrand.cpp)

source_group("Source Files\\Math\\RNG" FILES ${Framework_MATH_RNG_SOURCES})

set(Framework_AGGREGATE_SOURCES
	${Framework_CORE_SOURCES}
	${Framework_IPC_SOURCES}
	${Framework_LOGGING_SOURCES}
	${Framework_MATH_SOURCES}
	${Framework_MATH_RNG_SOURCES})

# setup target
add_library(Framework STATIC ${Framework_AGGREGATE_HEADERS} ${Framework_AGGREGATE_SOURCES})

set_target_properties(Framework PROPERTIES FOLDER "Base")

install(TARGETS Framework DESTINATION lib)
