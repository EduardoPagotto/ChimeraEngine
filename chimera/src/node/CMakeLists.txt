cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraNode)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourceNode Node.cpp
				Light.cpp
				Camera.cpp
				Material.cpp
				Solid.cpp
				PhysicsControl.cpp
				Constraint.cpp
				Texture.cpp
				Draw.cpp
				Mesh.cpp
				Font.cpp
				HUD.cpp
				SceneMng.cpp
				Particle.cpp
				ParticleEmitter.cpp
				ShadowMap.cpp
				Group.cpp
				Transform.cpp
				InitVisitor.cpp
				RenderVisitor.cpp
				MeshUtil.cpp
				ShadowMapVisitor.cpp
				NodeParse.cpp
				TextureManager.cpp)

#define os includes
set (IncludeNode ${CMAKE_SOURCE_DIR}) # Raiz do projeto
set (IncludeNodeAux ${OPENGL_INCLUDE_DIR}
				 	${SDL2_INCLUDE_DIR}
				 	${BULLET_INCLUDE_DIR}
				 	${FREETYPE_INCLUDE_DIRS}
				 	${GLEW_INCLUDE_DIRS})

# Define o Target
add_library (ChimeraNode STATIC ${SourceNode})

# Define os includes privados para este target
target_include_directories (ChimeraNode PUBLIC ${IncludeNode}
										PRIVATE ${IncludeNodeAux})

# Lista os Includes
# get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

add_dependencies(ChimeraNode ChimeraCore)

INSTALL(FILES ${ChimeraNode_BINARY_DIR}/libChimeraNode.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeNode}/chimera/node DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")