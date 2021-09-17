cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraCore)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourcesCore windows/CanvasGL.cpp
				 windows/CanvasFB.cpp
				 #windows/CanvasOVR.cpp
				 TrackBall.cpp
				 TrackHead.cpp
				 Shader.cpp
				 VertexData.cpp
				 Transform.cpp
				 io/utils.cpp
				 io/Timer.cpp
				 io/JoystickState.cpp
				 io/JoystickManager.cpp
				 io/MouseDevice.cpp
				 io/FlowControl.cpp
				 buffers/VertexBuffer.cpp
				 buffers/IndexBuffer.cpp
				 buffers/VertexArray.cpp)

# #define os includes
set (IncludeCore ${CMAKE_SOURCE_DIR})
set (IncludeCoreAux ${OPENGL_INCLUDE_DIR}
				 	${SDL2_INCLUDE_DIR}
				 	${GLEW_INCLUDE_DIRS})

#Adiciona objetos e includes do OVR ao SourcesCore e IncludeCore
# if (OVR_SET_TO_USE)
# 	message(STATUS "OVR Incluido no Projeto")
# 	set(SourcesCore ${SourcesCore} src/OvrDevice.cpp) #adiciona o cpp
# 	set(IncludeCore ${IncludeCore} /usr/local/ovr_sdk_linux_0.4.4/LibOVR) #adiciona o h
# endif(OVR_SET_TO_USE)

if (HMD_Z1_SET_TO_USE)
	message(STATUS "HMD_Z1 Incluido no Projeto")
	set(SourcesCore ${SourcesCore} windows/CanvasHmd.cpp) #adiciona o cpp
	set(SourcesCore ${SourcesCore} windows/Eye.cpp) #adiciona o cpp
endif(HMD_Z1_SET_TO_USE)

# Define o Target
add_library (ChimeraCore STATIC ${SourcesCore})

# Define os includes privados para este target
target_include_directories (ChimeraCore PUBLIC ${IncludeCore} 
										PRIVATE ${IncludeCoreAux})

# Lista os Includes
# get_property(dirs DIRECTORY ${IncludeCore}/chimera/core PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

INSTALL(FILES ${ChimeraCore_BINARY_DIR}/libChimeraCore.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeCore}/chimera/core DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
