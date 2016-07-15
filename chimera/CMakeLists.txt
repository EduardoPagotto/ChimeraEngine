PROJECT(Chimera)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)
#find_package(Log4Cxx REQUIRED)

set(INCLUDE_DIRECTORIES ${Chimera}/include/)

include_directories(
	${Chimera_SOURCE_DIR}/include
	${OPENGL_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	/usr/local/include/
	${BULLET_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${FTGL_INCLUDE_DIR}
	#${LOG4CXX_INCLUDE_DIR}
)

if (OVR_SET_TO_USE)
	include_directories(/usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)


get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
	message(STATUS "Include dir='${dir}'")
endforeach()

set (SourcesChimeraAPI
	src/ChimeraUtils.cpp
	src/Color.cpp
	src/Entity.cpp
	src/Node.cpp
	src/Light.cpp
	src/Object.cpp
	src/Camera.cpp
	src/Material.cpp
	src/CameraSpherical.cpp
	src/CameraMan.cpp
	src/CameraOvr.cpp
	src/Physics.cpp
	src/PhysicsControl.cpp
	src/Constraint.cpp
	src/Texture.cpp
	src/Draw.cpp
	src/DrawTriMesh.cpp
	src/Font.cpp
	src/HUD.cpp
	src/Timer.cpp
	src/Video.cpp
	src/VideoDevice.cpp
	src/LoaderDae.cpp
	src/SceneMng.cpp
	src/FlowControl.cpp
	src/GameClient.cpp
	src/JoystickManager.cpp
	src/JoystickState.cpp
	src/Particle.cpp
	src/ParticleEmitter.cpp
	src/ShadowMap.cpp
	src/Shader.cpp
	src/SceneRoot.cpp
	src/NodeVisitor.cpp
	src/Group.cpp
	src/State.cpp
	src/Transform.cpp
)
	
if (OVR_SET_TO_USE)
	message(STATUS "OVR Incluido no Projeto")
	set(SourcesChimeraAPI ${SourcesChimeraAPI} src/OvrDevice.cpp)
endif(OVR_SET_TO_USE)

add_library (Chimera STATIC ${SourcesChimeraAPI})

INSTALL(FILES ${Chimera_BINARY_DIR}/libChimera.a DESTINATION lib)
