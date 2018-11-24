cmake_minimum_required(VERSION 3.10.2)

PROJECT(Chimera)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourcesChimeraAPI 	src/Color.cpp
						src/Node.cpp
						src/Light.cpp
						src/Camera.cpp
						src/Material.cpp
						src/CameraSpherical.cpp
						src/CameraMan.cpp
						src/CameraOvr.cpp
						src/Solid.cpp
						src/PhysicsControl.cpp
						src/Constraint.cpp
						src/Texture.cpp
						src/Draw.cpp
						src/Mesh.cpp
						src/Font.cpp
						src/HUD.cpp
						src/Timer.cpp
						src/Video.cpp
						src/VideoDevice.cpp
						src/SceneMng.cpp
						src/FlowControl.cpp
						src/GameClient.cpp
						src/JoystickManager.cpp
						src/JoystickState.cpp
						src/Particle.cpp
						src/ParticleEmitter.cpp
						src/ShadowMap.cpp
						src/Shader.cpp
						src/Group.cpp
						src/State.cpp
						src/Transform.cpp
						src/VboIndexer.cpp
						src/InitVisitor.cpp
						src/RenderVisitor.cpp
						src/MeshUtil.cpp
						src/ShadowMapVisitor.cpp
						src/NodeParse.cpp
						src/TextureManager.cpp)

#define os includes
set (IncludesChimeraAPI ${CMAKE_CURRENT_SOURCE_DIR}/include
						${OPENGL_INCLUDE_DIR}
						${SDL2_INCLUDE_DIR}
						${BULLET_INCLUDE_DIR}
						${FREETYPE_INCLUDE_DIRS}
						${GLEW_INCLUDE_DIRS})

#Adiciona objetos e includes do OVR ao SourcesChimeraAPI e IncludesChimeraAPI
if (OVR_SET_TO_USE)
	message(STATUS "OVR Incluido no Projeto")
	set(SourcesChimeraAPI ${SourcesChimeraAPI} src/OvrDevice.cpp) #adiciona o cpp
	set(IncludesChimeraAPI ${IncludesChimeraAPI} /usr/local/ovr_sdk_linux_0.4.4/LibOVR) #adiciona o h
endif(OVR_SET_TO_USE)

if (HMD_Z1_SET_TO_USE)
	message(STATUS "HMD_Z1 Incluido no Projeto")
	set(SourcesChimeraAPI ${SourcesChimeraAPI} src/HmdDevice.cpp) #adiciona o cpp
endif(HMD_Z1_SET_TO_USE)

# Define o Target
add_library (Chimera STATIC ${SourcesChimeraAPI})

# Define os includes privados para este target
target_include_directories (Chimera PRIVATE ${IncludesChimeraAPI})

# Lista os Includes
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
	message(STATUS "Include dir='${dir}'")
endforeach()

INSTALL(FILES ${Chimera_BINARY_DIR}/libChimera.a DESTINATION lib)
