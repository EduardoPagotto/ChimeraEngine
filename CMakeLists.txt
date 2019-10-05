cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraEngine)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set(OVR_SET_TO_USE OFF)
set(HMD_Z1_SET_TO_USE ON)

#option(OVR_SET_TO_USE "Compilar com Driver OVR" ON)
#option(OVR_SET_TO_USE "Compilar sem Driver OVR" OFF)

if (OVR_SET_TO_USE)
	add_definitions(-DOVR_SET_TO_USE)
endif(OVR_SET_TO_USE)

if (HMD_Z1_SET_TO_USE)
	add_definitions(-DHMD_Z1_SET_TO_USE)
endif(HMD_Z1_SET_TO_USE)

set (OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(chimera)
add_subdirectory(examples)