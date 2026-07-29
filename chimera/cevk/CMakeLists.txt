cmake_minimum_required(VERSION 4.3.0)

PROJECT(cevk VERSION 0.0.1 LANGUAGES CXX)

find_package(SDL3 REQUIRED)
find_package(assimp REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)

# Compilar SpirV
# find_program(GLSLANG_VALIDATOR "glslangValidator")
# if(NOT GLSLANG_VALIDATOR)
#     message(FATAL_ERROR "glslangValidator não encontrado. Instale o Vulkan SDK.")
# endif()

# set(SHADER_SOURCES
#     ./shaders/shader.vert
#     ./shaders/shader.frag
# )

# set(SPV_OUTPUTS "./bin")

message(STATUS "Compiling: ${PROJECT_NAME}")
message(STATUS "Base: ${CMAKE_SOURCE_DIR}")
message(STATUS "Current: ${CMAKE_CURRENT_SOURCE_DIR}")

set (IncludeDirs ${CMAKE_CURRENT_SOURCE_DIR}/include/cevk)

set(SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/src/cevk.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/DevVK.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/debug.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptorPool.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptorSet.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptorSetLayout.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Buffers.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/VBO.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/IBO.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Image.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/CmdBuffer.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/CmdRender.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/CmdPool.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Textures.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/MeshModel.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Sync.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/SwapChain.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Pipeline.cpp)

add_library(${PROJECT_NAME} STATIC ${SourceFiles})

target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
