cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraPhysic)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourceNode Solid.cpp
				PhysicsControl.cpp
				Constraint.cpp)

#define os includes
set (IncludeNode ${CMAKE_SOURCE_DIR}) # Raiz do projeto
set (IncludeNodeAux ${OPENGL_INCLUDE_DIR}
				 	${SDL2_INCLUDE_DIR}
				 	${BULLET_INCLUDE_DIR}
				 	${GLEW_INCLUDE_DIRS})

# Define o Target
add_library (ChimeraPhysic STATIC ${SourceNode})

# Define os includes privados para este target
target_include_directories (ChimeraPhysic PUBLIC ${IncludeNode}
										  PRIVATE ${IncludeNodeAux})

add_dependencies(ChimeraPhysic ChimeraCore 
							   ChimeraRender
							   ChimeraNode)

INSTALL(FILES ${ChimeraPhysic_BINARY_DIR}/libChimeraPhysic.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeNode}/chimera/node DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
