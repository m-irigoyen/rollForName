CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#find_package(Boost 1.61 REQUIRED)

SET(EXE_NAME "Tests")

#Test project
PROJECT(${EXE_NAME})

FIND_PACKAGE( Boost 1.61 REQUIRED)

#Source grouping
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
FILE(GLOB_RECURSE SRC_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp )

FILE(GLOB_RECURSE SRC_TESTS
	${CMAKE_CURRENT_SOURCE_DIR}/src/Test.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Test.h 
	)
	
FILE(GLOB_RECURSE SRC_ENGINE
	${CMAKE_CURRENT_SOURCE_DIR}/../engine/rollForName/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/../engine/rollForName/*.h
	)
	
source_group(stuff FILES ${SRC_TESTS})

include_directories(${Boost_INCLUDE_DIRS}) 
add_executable(${EXE_NAME} ${SRC_MAIN} ${SRC_TESTS} ${SRC_ENGINE})