PROJECT(chimera_core LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(PUGIXML REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

message(STATUS "Compiling: ${PROJECT_NAME}")
message(STATUS "Base: ${CMAKE_SOURCE_DIR}")
message(STATUS "Current: ${CMAKE_CURRENT_SOURCE_DIR}")

# define os includes
set (IncludeDirs ${CMAKE_SOURCE_DIR}/chimera/chimera_base/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_core/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_space/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_ecs/include/
				 ${SDL3_INCLUDE_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${PUGIXML_DIR}
				 ${GLEW_INCLUDE_DIRS})

# define objetos a compilar
set (SourceFiles src/gl/CanvasGL.cpp
				 src/gl/buffer/IndexBuffer.cpp
				 src/gl/buffer/VertexArray.cpp
				 src/gl/buffer/FrameBuffer.cpp
				 src/gl/buffer/RenderBuffer.cpp
				 src/gl/DrawLine.cpp
				 src/gl/Shader.cpp
				 src/gl/ShaderMng.cpp
				 src/gl/Texture.cpp
				 src/gl/Font.cpp
				 src/gl/Material.cpp
				 src/gl/ParticleEmitter.cpp
				 src/gl/Wavefront.cpp
				 src/visible/Mesh.cpp
				 src/visible/CameraControllerFPS.cpp
				 src/visible/CameraControllerOrbit.cpp
				 src/partition/BSPTree.cpp
				 src/partition/LoadHeightMap.cpp
				 src/partition/Cube.cpp)

# Adiciona objetos e includes do OVR ao SourceFiles e IncludeDirs
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build ${PROJECT_NAME}")
	set(SourceFiles ${SourceFiles} device/CanvasOVR)
	set(SourceFiles ${SourceFiles} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)

if (BULLET_SET_TO_USE)
	message(STATUS "Bullet incluido no build ${PROJECT_NAME}")
	set(SourceFiles ${SourceFiles} src/bullet/PhysicsControl.cpp
								   src/bullet/Solid.cpp)
	set(IncludeDirs ${IncludeDirs} ${BULLET_INCLUDE_DIR} )
endif(BULLET_SET_TO_USE)

add_library (${PROJECT_NAME} STATIC ${SourceFiles})

target_include_directories (${PROJECT_NAME} PUBLIC ${IncludeDirs})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_dependencies(${PROJECT_NAME} clean-coverage chimera_base)
# Lista os Includes
# get_property(dirs DIRECTORY ${IncludeDirs}/chimera/core PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

#INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/libChimeraCore.a DESTINATION lib)
#INSTALL(DIRECTORY ${IncludeDirs}/chimera/core DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
