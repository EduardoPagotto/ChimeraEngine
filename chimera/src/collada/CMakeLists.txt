cmake_minimum_required(VERSION 3.18.2)

PROJECT(ChimeraCollada)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PUGIXML REQUIRED)
find_package(GLEW REQUIRED)

if (BULLET_SET_TO_USE)
	find_package(Bullet REQUIRED)
endif(BULLET_SET_TO_USE)

# define objetos a compilar
set (SourcesCore RFC3986.cpp
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

# define os includes
set (IncludeCore ${CMAKE_SOURCE_DIR}
				 ${SDL2_INCLUDE_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${PUGIXML_DIR}
				 ${GLEW_INCLUDE_DIRS})

# Adiciona objetos e includes do OVR ao SourcesCore e IncludeCore
# if (OVR_SET_TO_USE)
# 	message(STATUS "OVR incluido no build ChimeraCollada")
# 	set(SourcesCore ${SourcesCore} device/CanvasOVR) 
# 	set(SourcesCore ${SourcesCore} /usr/local/ovr_sdk_linux_0.4.4/LibOVR) 
# endif(OVR_SET_TO_USE)

if (BULLET_SET_TO_USE)	
	message(STATUS "Bullet incluido no build ChimeraCollada")
	set(SourcesCore ${SourcesCore} ColladaPhysicScene.cpp) 
	set(IncludeCore ${IncludeCore} ${BULLET_INCLUDE_DIR} )
endif(BULLET_SET_TO_USE)


# Define o Target
add_library (ChimeraCollada STATIC ${SourcesCore})

# Define os includes 
target_include_directories (ChimeraCollada PUBLIC ${IncludeCore})

# Lista os Includes
# get_property(dirs DIRECTORY ${IncludeCore}/chimera/core PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()
add_dependencies(ChimeraCollada ChimeraCore)

INSTALL(FILES ${ChimeraCollada_BINARY_DIR}/libChimeraCollada.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeCore}/chimera/core DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
