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
				 ColladaExtra.cpp)

# define os includes
set (IncludeCore ${CMAKE_SOURCE_DIR}/chimera
				 ${SDL2_INCLUDE_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${PUGIXML_DIR}
				 ${GLEW_INCLUDE_DIRS})

# Define o Target
add_library (ChimeraCollada STATIC ${SourcesCore})

# Define os includes
target_include_directories (ChimeraCollada PUBLIC ${IncludeCore})

add_dependencies(ChimeraBullet ChimeraCore
                               ChimeraSpace
							   ChimeraCoreGL)
