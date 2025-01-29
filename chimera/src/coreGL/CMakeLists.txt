cmake_minimum_required(VERSION 3.28)

PROJECT(ChimeraCoreGL)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

# define objetos a compilar
set (SourcesCore CanvasGL.cpp
				IndexBuffer.cpp
				VertexArray.cpp
				FrameBuffer.cpp
				RenderBuffer.cpp
				DrawLine.cpp
				Shader.cpp
				ShaderMng.cpp
				Texture.cpp
				CameraControllerFPS.cpp
				CameraControllerOrbit.cpp
				Font.cpp
				Material.cpp
				ParticleEmitter.cpp)

# define os includes
set (IncludeCore ${CMAKE_SOURCE_DIR}/chimera
				 ${SDL2_INCLUDE_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${GLEW_INCLUDE_DIRS}
				 )

# Adiciona objetos e includes do OVR ao SourcesCore e IncludeCore
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build ChimeraCore")
	set(SourcesCore ${SourcesCore} CanvasOVR)
	set(SourcesCore ${SourcesCore} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)

# Define o Target
add_library (ChimeraCoreGL STATIC ${SourcesCore})

# Define os includes
target_include_directories (ChimeraCoreGL PUBLIC ${IncludeCore})

add_dependencies(ChimeraCoreGL ChimeraCore
                               ChimeraSpace)
