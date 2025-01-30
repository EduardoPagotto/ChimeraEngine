cmake_minimum_required(VERSION 3.28)

PROJECT(ChimeraCollada)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PUGIXML REQUIRED)
find_package(GLEW REQUIRED)

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
				 ColladaPhysicScene.cpp)

# define os includes
set (IncludeCore ${CMAKE_SOURCE_DIR}/chimera
				 ${SDL2_INCLUDE_DIR}
				 ${BULLET_INCLUDE_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${PUGIXML_DIR}
				 ${GLEW_INCLUDE_DIRS})

# if (BULLET_SET_TO_USE)
# 	message(STATUS "Bullet incluido no build ChimeraCore")
# 	set(SourcesCore ${SourcesCore} ColladaPhysicScene.cpp)
# 	set(IncludeCore ${IncludeCore} ${BULLET_INCLUDE_DIR} )
# endif(BULLET_SET_TO_USE)


# Define o Target
add_library (ChimeraCollada STATIC ${SourcesCore})

# Define os includes
target_include_directories (ChimeraCollada PUBLIC ${IncludeCore})

add_dependencies(ChimeraCollada ChimeraCore
							    ChimeraSpace
							    ChimeraBullet
							    ChimeraCoreGL)
