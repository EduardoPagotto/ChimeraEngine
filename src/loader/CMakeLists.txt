cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraLoaders)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourcesLoader Library.cpp
				   LibraryPhysicsMaterials.cpp
				   LibraryImages.cpp
				   LibraryLights.cpp
				   LibraryCameras.cpp
				   LibraryEffects.cpp
				   LibraryMaterials.cpp
				   LibraryGeometrys.cpp
				   LibraryVisualSCenes.cpp
				   LibraryPhysicModels.cpp
				   LibraryPhysicsScenes.cpp
				   VisualScene.cpp
				   PhysicsScene.cpp
				   ListNodes.cpp
				   ShadersLoader.cpp)

#define os includes
set (IncludeLoader  ${CMAKE_SOURCE_DIR}/libs
					${OPENGL_INCLUDE_DIR}
					${SDL2_INCLUDE_DIR}
					${BULLET_INCLUDE_DIR}
					${FREETYPE_INCLUDE_DIRS}
					${GLEW_INCLUDE_DIRS})

# Define o Target
add_library (ChimeraLoaders STATIC ${SourcesLoader})

# Define os includes privados para este target
target_include_directories (ChimeraLoaders PRIVATE ${IncludeLoader})

# Lista os Includes
# get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

add_dependencies(ChimeraLoaders ChimeraCore)

INSTALL(FILES ${Chimera_BINARY_DIR}/libChimeraLoaders.a DESTINATION lib)
