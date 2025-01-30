cmake_minimum_required(VERSION 3.28)

PROJECT(ChimeraSpace)

find_package(SDL2 REQUIRED)

# define objetos a compilar
set (SourcesCore Mesh.cpp
				 BSPTree.cpp
				 LoadHeightMap.cpp
				 Cube.cpp)

# define os includes
set (IncludeCore ${CMAKE_SOURCE_DIR}/chimera
				 ${SDL2_INCLUDE_DIR})

# Define o Target
add_library (ChimeraSpace STATIC ${SourcesCore})

# Define os includes
target_include_directories (ChimeraSpace PUBLIC ${IncludeCore})

add_dependencies(ChimeraSpace ChimeraCore)

INSTALL(FILES ${ChimeraSpace_BINARY_DIR}/libChimeraSpace.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeCore}/chimera/space DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
