cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraGeneral)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (GeneralSource 	Plane.cpp
					Frustum.cpp
					AABB.cpp)

#define os includes
set (GeneralInclude ${CMAKE_SOURCE_DIR}) # Raiz do projeto
set (GeneralIncludeAux ${OPENGL_INCLUDE_DIR}
				 	${SDL2_INCLUDE_DIR}
				 	${GLEW_INCLUDE_DIRS})

# Define o Target
add_library (ChimeraGeneral STATIC ${GeneralSource})

# Define os includes privados para este target
target_include_directories (ChimeraGeneral PUBLIC ${GeneralInclude}
										  PRIVATE ${GeneralIncludeAux})

add_dependencies(ChimeraGeneral ChimeraCore)

INSTALL(FILES ${ChimeraGeneral_BINARY_DIR}/libChimeraGeneral.a DESTINATION lib)
INSTALL(DIRECTORY ${GeneralInclude}/chimera/node DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
