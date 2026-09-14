PROJECT(chimera_collada LANGUAGES CXX)

find_package(SDL3 REQUIRED)
find_package(PUGIXML REQUIRED)

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
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_collada/include/
				 ${SDL3_INCLUDE_DIR}
				 ${PUGIXML_DIR})

# define objetos a compilar
set (SourceFiles src/RFC3986.cpp
				 src/Collada.cpp
				 src/ColladaVisualScene.cpp
				 src/ColladaImage.cpp
				 src/ColladaMaterial.cpp
				 src/ColladaEffect.cpp
				 src/ColladaLight.cpp
				 src/ColladaCam.cpp
				 src/ColladaGeometry.cpp
				 src/colladaLoad.cpp
				 src/ColladaWaveFront.cpp
				 src/ColladaHeightMap.cpp
				 src/ColladaCube.cpp
				 src/ColladaMesh.cpp
				 src/ColladaParticle.cpp
				 src/ColladaExtra.cpp
				 src/ColladaRender.cpp)

# Adiciona objetos e includes do OVR ao SourceFiles e IncludeDirs
if (OVR_SET_TO_USE)
	message(STATUS "OVR incluido no build ${PROJECT_NAME}")
	set(SourceFiles ${SourceFiles} device/CanvasOVR)
	set(SourceFiles ${SourceFiles} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)

# add bullet engine
if (BULLET_SET_TO_USE)
	message(STATUS "Bullet incluido no build ${PROJECT_NAME}")
	set(SourceFiles ${SourceFiles} src/ColladaPhysicScene.cpp)
	set(IncludeDirs ${IncludeDirs} ${BULLET_INCLUDE_DIR} )
endif(BULLET_SET_TO_USE)

add_library (${PROJECT_NAME} STATIC ${SourceFiles})

target_include_directories (${PROJECT_NAME} PUBLIC ${IncludeDirs})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_dependencies(${PROJECT_NAME} chimera_core)

#INSTALL(FILES ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} DESTINATION lib)
#INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/chimera/core DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera/core FILES_MATCHING PATTERN "*.h*")
