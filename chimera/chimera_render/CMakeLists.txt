PROJECT(chimera_render LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

message(STATUS "Compiling: ${PROJECT_NAME}")
message(STATUS "Base: ${CMAKE_SOURCE_DIR}")
message(STATUS "Current: ${CMAKE_CURRENT_SOURCE_DIR}")

#define os includes
set (IncludeDirs ${CMAKE_SOURCE_DIR}/chimera/chimera_base/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_core/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_space/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_ecs/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_collada/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_render/include/
			     ${SDL3_INCLUDE_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${GLEW_INCLUDE_DIRS}
				 /usr/local/include/) # Raiz do projeto

#define objetos a compilar
set (SourceFiles 	src/2d/BatchRender2D.cpp
					src/2d/Layer.cpp
					src/3d/Renderer3d.cpp
					src/3d/Renderer3dLines.cpp
					src/3d/RenderableMesh.cpp
					src/3d/RenderableIBO.cpp
					src/3d/RenderableArray.cpp
					src/3d/RenderableBsp.cpp
					src/3d/RenderableDynamic.cpp
					src/3d/RenderableParticles.cpp
					src/scene/Scene.cpp)

# Adiciona objetos e includes do OVR ao SourceFiles e IncludeDirs
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build ${PROJECT_NAME}")
	set(IncludeDirs ${IncludeDirs} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)

if (BULLET_SET_TO_USE)
	message(STATUS "Bullet incluido no build ${PROJECT_NAME}")
	set(IncludeDirs ${IncludeDirs} ${BULLET_INCLUDE_DIR} )
endif(BULLET_SET_TO_USE)


add_library (${PROJECT_NAME} STATIC ${SourceFiles})

target_include_directories (${PROJECT_NAME} PUBLIC ${IncludeDirs})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_dependencies(${PROJECT_NAME} chimera_collada)

#INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/libChimeraRender.a DESTINATION lib)
#INSTALL(DIRECTORY ${IncludeDirs}/chimera/node DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
