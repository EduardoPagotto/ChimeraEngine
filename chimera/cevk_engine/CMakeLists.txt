cmake_minimum_required(VERSION 4.3.0)

PROJECT(cevk_engine VERSION 0.0.1 LANGUAGES CXX)

find_package(SDL3 REQUIRED)

message(STATUS "Compiling: ${PROJECT_NAME}")
message(STATUS "Base: ${CMAKE_SOURCE_DIR}")
message(STATUS "Current: ${CMAKE_CURRENT_SOURCE_DIR}")


set (IncludeDirs ${CMAKE_CURRENT_SOURCE_DIR}/include
                 ${CMAKE_SOURCE_DIR}/chimera/cevk/include
                 ${CMAKE_SOURCE_DIR}/chimera/cevk_infra/include)

set(SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/src/CanvaVK.cpp)

add_library(${PROJECT_NAME} STATIC ${SourceFiles})

target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_dependencies(${PROJECT_NAME} cevk_infra
                                 cevk)
