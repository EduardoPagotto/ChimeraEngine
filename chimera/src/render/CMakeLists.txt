cmake_minimum_required(VERSION 3.18.2)

PROJECT(ChimeraRender)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (RenderSource 	Renderable2D.cpp
					Sprite.cpp
					BatchRender2D.cpp
					Layer.cpp
					Label.cpp
					Tile.cpp
					Renderer3d.cpp
					Renderer3dLines.cpp
					Renderable3D.cpp
					RenderableMesh.cpp
					RenderableIBO.cpp
					RenderableArray.cpp
					RenderableBsp.cpp
					RenderableDynamic.cpp
					RenderableParticles.cpp
					Scene.cpp
					ColladaRender.cpp)

#define os includes
set (RenderInclude 	${CMAKE_SOURCE_DIR}
					${SDL2_INCLUDE_DIR}
					${OPENGL_INCLUDE_DIR}
					${GLEW_INCLUDE_DIRS}
					/usr/local/include/) # Raiz do projeto

# Adiciona objetos e includes do OVR ao RenderSource e RenderInclude
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build ChimeraRender")
	set(RenderInclude ${RenderInclude} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)

if (BULLET_SET_TO_USE)
	message(STATUS "Bullet incluido no build ChimeraRender")
	set(RenderInclude ${RenderInclude} ${BULLET_INCLUDE_DIR} )
endif(BULLET_SET_TO_USE)

# Define o Target
add_library (ChimeraRender STATIC ${RenderSource})

# Define os includes privados para este target
target_include_directories (ChimeraRender PUBLIC ${RenderInclude})

add_dependencies(ChimeraRender ChimeraCore)

INSTALL(FILES ${ChimeraRender_BINARY_DIR}/libChimeraRender.a DESTINATION lib)
INSTALL(DIRECTORY ${RenderInclude}/chimera/node DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
