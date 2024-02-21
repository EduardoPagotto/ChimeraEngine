cmake_minimum_required(VERSION 3.18.2)

PROJECT(ChimeraCore)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PUGIXML REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

# define objetos a compilar
set (SourcesCore Registry.cpp
				 utils.cpp
				 Engine.cpp
				 CanvasFB.cpp
				 CanvasGL.cpp
				 IndexBuffer.cpp
				 VertexArray.cpp
				 FrameBuffer.cpp
				 RenderBuffer.cpp
				 DrawLine.cpp
				 Mesh.cpp
				 Shader.cpp
				 Texture.cpp
				 TextureManager.cpp
				 CameraControllerFPS.cpp
				 CameraControllerOrbit.cpp
				 FontAtlas.cpp
				 FontManager.cpp
				 Material.cpp
				 ParticleEmitter.cpp
				 #RFC3986.cpp
				 #Collada.cpp
				 collada/ColladaVisualScene.cpp
				 collada/ColladaImage.cpp
				 collada/ColladaMaterial.cpp
				 collada/ColladaEffect.cpp
				 collada/ColladaLight.cpp
				 collada/ColladaCam.cpp
				 collada/ColladaGeometry.cpp
				 #collada/colladaLoad.cpp
				 collada/ColladaWaveFront.cpp
				 collada/ColladaHeightMap.cpp
				 collada/ColladaCube.cpp
				 collada/ColladaMesh.cpp
				 collada/ColladaParticle.cpp
				 collada/ColladaExtra.cpp
				 BSPTree.cpp
				 LoadHeightMap.cpp
				 Wavefront.cpp
				 Cube.cpp)

# define os includes
set (IncludeCore ${CMAKE_SOURCE_DIR}
				 ${SDL2_INCLUDE_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${PUGIXML_DIR}
				 ${GLEW_INCLUDE_DIRS})

# Adiciona objetos e includes do OVR ao SourcesCore e IncludeCore
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build ChimeraCore")
	set(SourcesCore ${SourcesCore} CanvasOVR) 
	set(SourcesCore ${SourcesCore} /usr/local/ovr_sdk_linux_0.4.4/LibOVR) 
endif(OVR_SET_TO_USE)

if (BULLET_SET_TO_USE)	
	message(STATUS "Bullet incluido no build ChimeraCore")
	set(SourcesCore ${SourcesCore} PhysicsControl.cpp
								   Solid.cpp
								   collada/ColladaPhysicScene.cpp
								   PhysicController.cpp) 
	set(IncludeCore ${IncludeCore} ${BULLET_INCLUDE_DIR} )
endif(BULLET_SET_TO_USE)


# Define o Target
add_library (ChimeraCore SHARED ${SourcesCore})

# Define os includes 
target_include_directories (ChimeraCore PUBLIC ${IncludeCore})

# Lista os Includes
# get_property(dirs DIRECTORY ${IncludeCore}/chimera/core PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

INSTALL(FILES ${ChimeraCore_BINARY_DIR}/libChimeraCore.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeCore}/chimera/core DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
