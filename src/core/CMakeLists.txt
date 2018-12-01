cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraCore)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourcesCore Video.cpp
				 VideoDevice.cpp
				 Timer.cpp
				 JoystickState.cpp
				 JoystickManager.cpp
				 Shader.cpp	
				 GameClient.cpp
				 FlowControl.cpp
				 State.cpp)

# #define os includes
set (IncludeCore ${CMAKE_SOURCE_DIR}/libs #../../libs
				 ${OPENGL_INCLUDE_DIR}
				 ${SDL2_INCLUDE_DIR}
				 ${GLEW_INCLUDE_DIRS})

#Adiciona objetos e includes do OVR ao SourcesCore e IncludeCore
if (OVR_SET_TO_USE)
	message(STATUS "OVR Incluido no Projeto")
	set(SourcesCore ${SourcesCore} src/OvrDevice.cpp) #adiciona o cpp
	set(IncludeCore ${IncludeCore} /usr/local/ovr_sdk_linux_0.4.4/LibOVR) #adiciona o h
endif(OVR_SET_TO_USE)

if (HMD_Z1_SET_TO_USE)
	message(STATUS "HMD_Z1 Incluido no Projeto")
	set(SourcesCore ${SourcesCore} HmdDevice.cpp) #adiciona o cpp
endif(HMD_Z1_SET_TO_USE)

# Define o Target
add_library (ChimeraCore STATIC ${SourcesCore})

# Define os includes privados para este target
target_include_directories (ChimeraCore PRIVATE ${IncludeCore})

## Lista os Includes
# get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

INSTALL(FILES ${Chimera_BINARY_DIR}/libChimeraCore.a DESTINATION lib)
