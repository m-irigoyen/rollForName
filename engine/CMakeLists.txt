CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE( Boost 1.61 REQUIRED)

SET(LIB_NAME "RollForName")

set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ENGINE_INCLUDE_DIR ${SHARED_LIB_INCLUDE_DIR} PARENT_SCOPE)

if(TARGET ${LIB_NAME})
	message("shared_lib is already defined")
else()
	include_directories(${ENGINE_INCLUDE_DIR})

	FILE(GLOB_RECURSE SRC_include
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/include/rollForName/*
		)
	
	FILE(GLOB_RECURSE SRC_ALL
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/src/*
		)

	include_directories(${Boost_INCLUDE_DIRS}) 
	add_library(${LIB_NAME} STATIC ${SRC_ALL} ${SRC_include})
endif()
