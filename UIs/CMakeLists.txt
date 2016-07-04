CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#find_package(Boost 1.61 REQUIRED)

SET(EXE_NAME "rollForNameConsole")

#Test project
PROJECT(${EXE_NAME})

add_subdirectory(./../engine engine)
include_directories(../engine/include)

#Source grouping
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
FILE(GLOB_RECURSE SRC_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp )

include_directories(${SRC_ENGINE})
add_executable(${EXE_NAME} ${SRC_MAIN})
target_link_libraries(${EXE_NAME} RollForName)