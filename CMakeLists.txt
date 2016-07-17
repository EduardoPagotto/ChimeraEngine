PROJECT(ChimeraEngine)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set(OVR_SET_TO_USE OFF)
#option(OVR_SET_TO_USE "Compilar com Driver OVR" ON)
#option(OVR_SET_TO_USE "Compilar sem Driver OVR" OFF)

if (OVR_SET_TO_USE)
	add_definitions(-DOVR_SET_TO_USE)
endif(OVR_SET_TO_USE)

add_subdirectory(chimera)
add_subdirectory(AppCargaManual)
add_subdirectory(AppEmpty)
add_subdirectory(AppShader)
#add_subdirectory(appTest)
