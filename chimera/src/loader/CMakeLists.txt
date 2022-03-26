cmake_minimum_required(VERSION 3.18.2)

PROJECT(ChimeraLoaders)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourcesLoader Library.cpp
				   LibraryImages.cpp
				   LibraryLights.cpp
				   LibraryCameras.cpp
				   LibraryEffects.cpp
				   LibraryMaterials.cpp
				   LibraryGeometrys.cpp
				   LibraryVisualSCenes.cpp
				   VisualScene.cpp)

#define os includes
set (IncludeLoader  ${CMAKE_SOURCE_DIR})
set (IncludeLoaderAux	${OPENGL_INCLUDE_DIR}
						${SDL2_INCLUDE_DIR}
						${GLEW_INCLUDE_DIRS})

if (BULLET_SET_TO_USE)
message(STATUS "Bullet incluido no build ChimeraLoaders")
set(SourcesLoader ${SourcesLoader} LibraryPhysicsMaterials.cpp
				   				   LibraryPhysicModels.cpp
								   LibraryPhysicsScenes.cpp
								   PhysicsScene.cpp) #adiciona o cpp
set(IncludeLoader ${IncludeLoader} ${BULLET_INCLUDE_DIR} ) #adiciona o h
endif(BULLET_SET_TO_USE)


# Define o Target
add_library (ChimeraLoaders STATIC ${SourcesLoader})

# Define os includes privados para este target
target_include_directories (ChimeraLoaders PUBLIC ${IncludeLoader}
										   PRIVATE ${IncludeLoaderAux})

add_dependencies(ChimeraLoaders ChimeraCore
								ChimeraRender)

INSTALL(FILES ${ChimeraLoaders_BINARY_DIR}/libChimeraLoaders.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeLoader}/chimera/loader DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
