cmake_minimum_required(VERSION 3.19)
project(Manta)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_CXX_COMPILER_ID}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}/")
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# Tells CMake to generate compile_commands.json
# set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_SHARED_LIBRARY_PREFIX "lib")

message("Build type = ${CMAKE_BUILD_TYPE}")

# idk why CMake doesn't do this automatically
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    # set(CMAKE_VERBOSE_MAKEFILE ON) # Our shiny red button of debugging
    add_compile_definitions("_DEBUG" "DEBUG")
endif()

# Common directories
set(PROJECTS_DIR ${CMAKE_SOURCE_DIR}/projects)
set(COMMON_DIR ${PROJECTS_DIR}/common)
set(ENGINE_DIR ${PROJECTS_DIR}/engine)
set(DEPENDS_DIR "${CMAKE_SOURCE_DIR}/dependencies")

# Engine
add_subdirectory(${ENGINE_DIR})