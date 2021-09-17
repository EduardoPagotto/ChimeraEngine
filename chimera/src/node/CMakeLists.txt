cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraNode)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourceNode Node.cpp
				NodeLight.cpp
				NodeCamera.cpp
				NodeMesh.cpp
				NodeHUD.cpp
				NodeParticleEmitter.cpp
				NodeGroup.cpp
				VisitorRender.cpp
				VisitorShadowMap.cpp
				VisitParser.cpp)

#define os includes
set (IncludeNode ${CMAKE_SOURCE_DIR}) # Raiz do projeto
set (IncludeNodeAux ${OPENGL_INCLUDE_DIR}
				 	${SDL2_INCLUDE_DIR}
				 	${BULLET_INCLUDE_DIR}
				 	${GLEW_INCLUDE_DIRS})

# Define o Target
add_library (ChimeraNode STATIC ${SourceNode})

# Define os includes privados para este target
target_include_directories (ChimeraNode PUBLIC ${IncludeNode}
										PRIVATE ${IncludeNodeAux})

add_dependencies(ChimeraNode ChimeraCore 
                             ChimeraRender)

INSTALL(FILES ${ChimeraNode_BINARY_DIR}/libChimeraNode.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeNode}/chimera/node DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
