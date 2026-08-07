cmake_minimum_required(VERSION 4.3.0)

PROJECT(loader_glft VERSION 0.0.1 LANGUAGES CXX)

# Antes do find_package, informe onde procurar
list(APPEND CMAKE_PREFIX_PATH "/usr/local")

find_package(SDL3 REQUIRED)
find_package(assimp REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(fastgltf REQUIRED)

set (IncludeDirs ${CMAKE_CURRENT_SOURCE_DIR}/include
                 ${CMAKE_SOURCE_DIR}/src/cevk/include/cevk)

set (LIBRARY_DEPS ${CMAKE_SOURCE_DIR}/bin/libcevk.a
                  SDL3::SDL3
                  Vulkan::Vulkan
                  GPUOpen::VulkanMemoryAllocator
                  assimp::assimp
                  fastgltf::fastgltf)

add_executable(${PROJECT_NAME}  ${CMAKE_CURRENT_SOURCE_DIR}/src/Loader.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY_DEPS})

#add_dependencies(${PROJECT_NAME} cevk)
