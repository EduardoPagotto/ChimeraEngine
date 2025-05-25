PROJECT(ChimeraCore LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PUGIXML REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

# define objetos a compilar
set (SourceFiles device/CanvasFB.cpp
				 gl/CanvasGL.cpp
				 gl/buffer/IndexBuffer.cpp
				 gl/buffer/VertexArray.cpp
				 gl/buffer/FrameBuffer.cpp
				 gl/buffer/RenderBuffer.cpp
				 gl/DrawLine.cpp
				 gl/Shader.cpp
				 gl/ShaderMng.cpp
				 gl/Texture.cpp
				 gl/Font.cpp
				 gl/Material.cpp
				 gl/ParticleEmitter.cpp
				 gl/Wavefront.cpp
				 visible/Mesh.cpp
				 visible/CameraControllerFPS.cpp
				 visible/CameraControllerOrbit.cpp
				 partition/BSPTree.cpp
				 partition/LoadHeightMap.cpp
				 partition/Cube.cpp)

# define os includes
set (IncludeDirs ${CMAKE_SOURCE_DIR}
				 ${SDL2_INCLUDE_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${PUGIXML_DIR}
				 ${GLEW_INCLUDE_DIRS})

# Adiciona objetos e includes do OVR ao SourceFiles e IncludeDirs
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build ${PROJECT_NAME}")
	set(SourceFiles ${SourceFiles} device/CanvasOVR)
	set(SourceFiles ${SourceFiles} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)

if (BULLET_SET_TO_USE)
	message(STATUS "Bullet incluido no build ${PROJECT_NAME}")
	set(SourceFiles ${SourceFiles} bullet/PhysicsControl.cpp
								   bullet/Solid.cpp)
	set(IncludeDirs ${IncludeDirs} ${BULLET_INCLUDE_DIR} )
endif(BULLET_SET_TO_USE)


# Define o Target
add_library (${PROJECT_NAME} STATIC ${SourceFiles})

# Define os includes
target_include_directories (${PROJECT_NAME} PUBLIC ${IncludeDirs})

# Lista os Includes
# get_property(dirs DIRECTORY ${IncludeDirs}/chimera/core PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

#INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/libChimeraCore.a DESTINATION lib)
#INSTALL(DIRECTORY ${IncludeDirs}/chimera/core DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
