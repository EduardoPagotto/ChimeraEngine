cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraRender)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (RenderSource 	vbs/RenderableIBO.cpp
					vbs/RenderableStatic.cpp
					vbs/SimpleRender3d.cpp
					vbs/RenderableIndex.cpp
					2d/Renderable2D.cpp
					2d/Sprite.cpp
					2d/BatchRender2D.cpp
					2d/layer/Layer.cpp
					2d/layer/Group.cpp
					2d/Label.cpp
					3d/Renderer3d.cpp
					3d/RenderableSimple.cpp
					3d/RenderableFace.cpp
					3d/RenderableChunk.cpp
					3d/RenderableBsp.cpp
					3d/RenderableDynamic.cpp
					Font.cpp
					FontAtlas.cpp
					FontManager.cpp
					Light.cpp
					Material.cpp
					Particle.cpp
					LoadObj.cpp
					LoadHeightMap.cpp
					maze/Cube.cpp 
					maze/Maze.cpp
					scene/Scene.cpp
					scene/CameraControllerFPS.cpp
					scene/CameraControllerOrbit.cpp)

#define os includes
set (RenderInclude ${CMAKE_SOURCE_DIR}
					/usr/local/include/) # Raiz do projeto
					
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
