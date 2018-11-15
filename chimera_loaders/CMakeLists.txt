cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraLoaders)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define objetos a compilar
set (SourcesChimeraLoadersAPI src/Library.cpp
							  src/LibraryPhysicsMaterials.cpp
							  src/LibraryImages.cpp
							  src/LibraryLights.cpp
							  src/LibraryCameras.cpp
							  src/LibraryEffects.cpp
							  src/LibraryMaterials.cpp
							  src/LibraryGeometrys.cpp
							  src/LibraryVisualSCenes.cpp
							  src/LibraryPhysicModels.cpp
							  src/LibraryPhysicsScenes.cpp
							  src/VisualScene.cpp
							  src/PhysicsScene.cpp
							  src/ListNodes.cpp)

#define os includes
set (IncludesChimeraLoadersAPI  ${CMAKE_CURRENT_SOURCE_DIR}/include
								${Chimera_SOURCE_DIR}/include
								${OPENGL_INCLUDE_DIR}
								${SDL2_INCLUDE_DIR}
								${BULLET_INCLUDE_DIR}
								${FREETYPE_INCLUDE_DIRS}
								${GLEW_INCLUDE_DIRS})

#Adiciona objetos e includes do OVR ao SourcesChimeraLoadersAPI e IncludesChimeraLoadersAPI
# if (OVR_SET_TO_USE)
# 	message(STATUS "OVR Incluido no Projeto")
# 	set(SourcesChimeraLoadersAPI ${SourcesChimeraLoadersAPI} src/OvrDevice.cpp) #adiciona o cpp
# 	set(IncludesChimeraLoadersAPI ${IncludesChimeraLoadersAPI} /usr/local/ovr_sdk_linux_0.4.4/LibOVR) #adiciona o h
# endif(OVR_SET_TO_USE)

# if (HMD_Z1_SET_TO_USE)
# 	message(STATUS "HMD_Z1 Incluido no Projeto")
# 	set(SourcesChimeraLoadersAPI ${SourcesChimeraLoadersAPI} src/HmdDevice.cpp) #adiciona o cpp
# endif(HMD_Z1_SET_TO_USE)

# Define o Target
add_library (ChimeraLoaders STATIC ${SourcesChimeraLoadersAPI})

# Define os includes privados para este target
target_include_directories (ChimeraLoaders PRIVATE ${IncludesChimeraLoadersAPI})

# Lista os Includes
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
	message(STATUS "Include dir='${dir}'")
endforeach()

add_dependencies(ChimeraLoaders Chimera)

INSTALL(FILES ${Chimera_BINARY_DIR}/libChimeraLoaders.a DESTINATION lib)
