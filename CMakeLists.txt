cmake_minimum_required(VERSION 4.3.0)

PROJECT(ChimeraEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS} -g -fprofile-instr-generate -fcoverage-mapping -fstandalone-debug")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

#if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Exemplo de comando para limpar os arquivos de cobertura antigos antes de rodar
    add_custom_target(clean-coverage
        COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/**/*.gcda"
        COMMENT "Limpando arquivos GCDA antigos..."
    )
#endif()

# # Locate the clang-tidy executable on the system
# find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
# if(CLANG_TIDY_EXE)
#     # This forces clang-tidy to run on all target source files whenever you build
#     set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE};-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy)
# else()
#     message(STATUS "clang-tidy not found.")
# endif()

# # inclui projetos externos
# include(FetchContent)

# # Configura o fastgltf e sua dependência obrigatória (simdjson)
# FetchContent_Declare(
#   fastgltf
#   GIT_REPOSITORY https://github.com/spnda/fastgltf
#   GIT_TAG        v0.9.0  # Verifique a release mais recente no GitHub
# )
# FetchContent_MakeAvailable(fastgltf)

# # Configura o entt
# FetchContent_Declare(
#   EnTT
#   GIT_REPOSITORY https://github.com/skypjack/entt
#   GIT_TAG        v3.13.2 # Substitua pela versão desejada
# )
# FetchContent_MakeAvailable(EnTT)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Engine

add_subdirectory(chimera/cevk)
add_subdirectory(chimera/cevk_engine)

add_subdirectory(chimera/chimera_base/)

# add_subdirectory(chimera/src/core)
# add_subdirectory(chimera/src/collada)
# add_subdirectory(chimera/src/render)

# Simple testes app

add_subdirectory(samples/sdl31)
#add_subdirectory(samples/test/sdl32)
# add_subdirectory(samples/test/loader_asset)
# #add_subdirectory(samples/test/glft_load)

#add_subdirectory(samples/loader)

add_subdirectory(samples/raycasting)
# add_subdirectory(samples/helloworld)
# add_subdirectory(samples/simples)
# add_subdirectory(samples/bsptree)
# add_subdirectory(samples/terrain)
# add_subdirectory(samples/models3)

# clean
add_custom_target (clean-all COMMAND rm -rf ${CMAKE_BINARY_DIR}/*
                             COMMAND rm -rf ${CMAKE_BINARY_DIR}/.cmake
                             COMMAND rm -rf ${CMAKE_BINARY_DIR}/.ninja_*
                             COMMAND rm -rf ${CMAKE_SOURCE_DIR}/bin/*)
