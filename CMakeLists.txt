cmake_minimum_required(VERSION 3.31)

PROJECT(ChimeraEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs -fstandalone-debug")

set(OVR_SET_TO_USE OFF)
set(BULLET_SET_TO_USE ON)

#option(OVR_SET_TO_USE "Compilar com Driver OVR" ON)
#option(OVR_SET_TO_USE "Compilar sem Driver OVR" OFF)

if (BULLET_SET_TO_USE)
	add_definitions(-DBULLET_SET_TO_USE)
endif(BULLET_SET_TO_USE)

if (OVR_SET_TO_USE)
	add_definitions(-DOVR_SET_TO_USE)
endif(OVR_SET_TO_USE)

set (OpenGL_GL_PREFERENCE GLVND)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

# make dynamic cfs
add_subdirectory(configured)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Engine
add_subdirectory(chimera/src/core)
add_subdirectory(chimera/src/collada)
add_subdirectory(chimera/src/render)

# Simple testes app
add_subdirectory(samples/raycasting)
add_subdirectory(samples/helloworld)
add_subdirectory(samples/simples)
add_subdirectory(samples/bsptree)
add_subdirectory(samples/terrain)
add_subdirectory(samples/models3)

# clean
add_custom_target (clean-all COMMAND rm -rf ${CMAKE_BINARY_DIR}/*
                             COMMAND rm -rf ${CMAKE_BINARY_DIR}/.cmake
                             COMMAND rm -rf ${CMAKE_BINARY_DIR}/.ninja_*
                             COMMAND rm -rf ${CMAKE_SOURCE_DIR}/bin/*)
