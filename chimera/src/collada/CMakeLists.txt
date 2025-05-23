PROJECT(ChimeraCollada LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PUGIXML REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

# define os includes
set (IncludeDirs ${CMAKE_SOURCE_DIR}
				 ${SDL2_INCLUDE_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${PUGIXML_DIR}
				 ${GLEW_INCLUDE_DIRS})

# define objetos a compilar
set (SourceFiles RFC3986.cpp
				 Collada.cpp
				 ColladaVisualScene.cpp
				 ColladaImage.cpp
				 ColladaMaterial.cpp
				 ColladaEffect.cpp
				 ColladaLight.cpp
				 ColladaCam.cpp
				 ColladaGeometry.cpp
				 colladaLoad.cpp
				 ColladaWaveFront.cpp
				 ColladaHeightMap.cpp
				 ColladaCube.cpp
				 ColladaMesh.cpp
				 ColladaParticle.cpp
				 ColladaExtra.cpp
				 ColladaRender.cpp)

# Adiciona objetos e includes do OVR ao SourceFiles e IncludeDirs
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build ${PROJECT_NAME}")
	set(SourceFiles ${SourceFiles} device/CanvasOVR)
	set(SourceFiles ${SourceFiles} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)

# add bullet engine
if (BULLET_SET_TO_USE)
	message(STATUS "Bullet incluido no build ${PROJECT_NAME}")
	set(SourceFiles ${SourceFiles} ColladaPhysicScene.cpp)
	set(IncludeDirs ${IncludeDirs} ${BULLET_INCLUDE_DIR} )
endif(BULLET_SET_TO_USE)

# Define o Target
add_library (${PROJECT_NAME} STATIC ${SourceFiles})

# Define os includes
target_include_directories (${PROJECT_NAME} PUBLIC ${IncludeDirs})

# dependencies
add_dependencies(${PROJECT_NAME} ChimeraCore)

#INSTALL(FILES ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} DESTINATION lib)
#INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/chimera/core DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera/core FILES_MATCHING PATTERN "*.h*")
