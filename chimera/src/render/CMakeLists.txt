cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraRender)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (RenderSource 	Triangle.cpp
					Plane.cpp
					Frustum.cpp
					AABB.cpp
					Octree.cpp
					BSPTree.cpp
					LoadHeightMap.cpp
					VertexNode.cpp
					VAO.cpp
					EBO.cpp
					VBO.cpp
					VertexData.cpp
					VertexDynamic.cpp 
					VertexStatic.cpp
					HeightMap.cpp
					LoadObj.cpp
					Light.cpp
					Material.cpp
					MeshData.cpp
					Texture.cpp)

#define os includes
set (RenderInclude ${CMAKE_SOURCE_DIR}) # Raiz do projeto
set (RenderIncludeAux ${OPENGL_INCLUDE_DIR}
				 	${SDL2_INCLUDE_DIR}
				 	${GLEW_INCLUDE_DIRS})

# Define o Target
add_library (ChimeraRender STATIC ${RenderSource})

# Define os includes privados para este target
target_include_directories (ChimeraRender PUBLIC ${RenderInclude}
										  PRIVATE ${RenderIncludeAux})

add_dependencies(ChimeraRender ChimeraCore)

INSTALL(FILES ${ChimeraRender_BINARY_DIR}/libChimeraRender.a DESTINATION lib)
INSTALL(DIRECTORY ${RenderInclude}/chimera/node DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
