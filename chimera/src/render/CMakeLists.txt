cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraRender)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (RenderSource 	CanvasGL.cpp
					Shader.cpp
					VertexData.cpp
					Texture.cpp
					TextureManager.cpp
					CameraFPS.cpp
					CameraOrbit.cpp
					CameraOrthographic.cpp
					Font.cpp
					FontAtlas.cpp
					FontManager.cpp
					Light.cpp
					Material.cpp
					Particle.cpp
					partition/Triangle.cpp
					partition/Plane.cpp
					partition/Frustum.cpp
					partition/AABB.cpp
					partition/Octree.cpp
					partition/BSPTree.cpp
					partition/Cube.cpp 
					partition/Maze.cpp
					partition/LoadObj.cpp
					partition/LoadHeightMap.cpp
					buffer/VertexBuffer.cpp
					buffer/IndexBuffer.cpp
					buffer/VertexArray.cpp
					buffer/FrameBuffer.cpp
					buffer/RenderBuffer.cpp
					vbs/RenderableStatic.cpp
					vbs/SimpleRender3d.cpp
					2d/Renderable2D.cpp
					2d/Sprite.cpp
					2d/BatchRender2D.cpp
					2d/layer/Layer.cpp
					2d/layer/Group.cpp
					2d/Label.cpp
					3d/PolygonIndex.cpp
					3d/Renderer3d.cpp
					3d/RenderableSimple.cpp
					3d/RenderableChunk.cpp
					3d/RenderableBsp.cpp
					3d/RenderableDynamic.cpp
					scene/Scene.cpp
					scene/CameraController.cpp
					scene/RenderPass.cpp
					scene/RenderPassShadow.cpp)

#define os includes
set (RenderInclude 	${CMAKE_SOURCE_DIR}
					${SDL2_INCLUDE_DIR}
					${OPENGL_INCLUDE_DIR}
					${GLEW_INCLUDE_DIRS}
					/usr/local/include/) # Raiz do projeto

# Adiciona objetos e includes do OVR ao RenderSource e RenderInclude
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build")
	set(RenderSource ${RenderSource} src/OvrDevice.cpp) #adiciona o cpp
	set(RenderInclude ${RenderInclude} /usr/local/ovr_sdk_linux_0.4.4/LibOVR) #adiciona o h
endif(OVR_SET_TO_USE)

if (BULLET_SET_TO_USE) # TODO: Remover para o Core depois de estrair o Transform	message(STATUS "Bullet incluido no build")
	set(RenderSource ${RenderSource} bullet/PhysicsControl.cpp
									 bullet/Solid.cpp
									 bullet/Constraint.cpp) #adiciona o cpp
	set(RenderInclude ${RenderInclude} ${BULLET_INCLUDE_DIR} ) #adiciona o h
endif(BULLET_SET_TO_USE)

if (HMD_Z1_SET_TO_USE)
	message(STATUS "HMD_Z1 incluido no build")
	set(RenderSource ${RenderSource} CanvasHmd.cpp) #adiciona o cpp
endif(HMD_Z1_SET_TO_USE)
				
# Define o Target
add_library (ChimeraRender STATIC ${RenderSource})

# Define os includes privados para este target
target_include_directories (ChimeraRender PUBLIC ${RenderInclude})

add_dependencies(ChimeraRender ChimeraCore)

INSTALL(FILES ${ChimeraRender_BINARY_DIR}/libChimeraRender.a DESTINATION lib)
INSTALL(DIRECTORY ${RenderInclude}/chimera/node DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
