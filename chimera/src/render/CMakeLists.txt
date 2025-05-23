PROJECT(ChimeraRender LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourceFiles 	2d/BatchRender2D.cpp
					2d/Layer.cpp
					3d/Renderer3d.cpp
					3d/Renderer3dLines.cpp
					3d/Renderable3D.cpp
					3d/RenderableMesh.cpp
					3d/RenderableIBO.cpp
					3d/RenderableArray.cpp
					3d/RenderableBsp.cpp
					3d/RenderableDynamic.cpp
					3d/RenderableParticles.cpp
					scene/Scene.cpp)

#define os includes
set (IncludeDirs 	${CMAKE_SOURCE_DIR}
					${SDL2_INCLUDE_DIR}
					${OPENGL_INCLUDE_DIR}
					${GLEW_INCLUDE_DIRS}
					/usr/local/include/) # Raiz do projeto

# Adiciona objetos e includes do OVR ao SourceFiles e IncludeDirs
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build ${PROJECT_NAME}")
	set(IncludeDirs ${IncludeDirs} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)

if (BULLET_SET_TO_USE)
	message(STATUS "Bullet incluido no build ${PROJECT_NAME}")
	set(IncludeDirs ${IncludeDirs} ${BULLET_INCLUDE_DIR} )
endif(BULLET_SET_TO_USE)

# Define o Target
add_library (${PROJECT_NAME} STATIC ${SourceFiles})

# Define os includes privados para este target
target_include_directories (${PROJECT_NAME} PUBLIC ${IncludeDirs})

add_dependencies(${PROJECT_NAME} ChimeraCollada)

#INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/libChimeraRender.a DESTINATION lib)
#INSTALL(DIRECTORY ${IncludeDirs}/chimera/node DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
