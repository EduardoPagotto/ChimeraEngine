cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraPhysicLoader)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourcesLoader 
				   LibraryPhysicsMaterials.cpp
				   LibraryPhysicModels.cpp
				   LibraryPhysicsScenes.cpp
				   PhysicsScene.cpp)

#define os includes
set (IncludeLoader  ${CMAKE_SOURCE_DIR})
set (IncludeLoaderAux	${OPENGL_INCLUDE_DIR}
						${SDL2_INCLUDE_DIR}
						${BULLET_INCLUDE_DIR}
						${FREETYPE_INCLUDE_DIRS}
						${GLEW_INCLUDE_DIRS})

# Define o Target
add_library (ChimeraPhysicLoader STATIC ${SourcesLoader})

# Define os includes privados para este target
target_include_directories (ChimeraPhysicLoader PUBLIC ${IncludeLoader}
										   PRIVATE ${IncludeLoaderAux})

# Lista os Includes
# get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

add_dependencies(ChimeraPhysicLoader ChimeraCore
								     ChimeraRender
									 ChimeraNode
									 ChimeraPhysic
									 ChimeraLoaders)

INSTALL(FILES ${ChimeraPhysicLoader_BINARY_DIR}/libChimeraPhysicLoader.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeLoader}/chimera/loader DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
