PROJECT(ChimeraEngine)

cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")


add_subdirectory(chimera)
add_subdirectory(AppCargaManual)
add_subdirectory(AppShader)
add_subdirectory(AppEmpty)
#add_subdirectory(appTest)
