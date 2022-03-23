cmake_minimum_required(VERSION 3.18.4)

PROJECT(ColladaDB)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PUGIXML REQUIRED) #PUGIXML
find_package(GLEW REQUIRED)

# objetos a compilar
set (SourceColladaDB	Collada.cpp
						ColladaVisualScene.cpp
						ColladaMaterial.cpp
						ColladaLight.cpp
						ColladaCam.cpp
						ColladaGeometry.cpp)

# Includes
set (IncludeColladaDB	${CMAKE_SOURCE_DIR}
				 		${SDL2_INCLUDE_DIR}
						${OPENGL_INCLUDE_DIR}
						${PUGIXML_DIR})

if (BULLET_SET_TO_USE)
message(STATUS "Bullet incluido no build")
set(SourceColladaDB ${SourceColladaDB} ColladaPhysicScene.cpp) #adiciona o cpp
set(IncludeColladaDB ${IncludeColladaDB} ${BULLET_INCLUDE_DIR} ) #adiciona o h
endif(BULLET_SET_TO_USE)

# Target
add_library (ColladaDB STATIC ${SourceColladaDB})

# adiciona os Includes 
target_include_directories (ColladaDB PUBLIC ${IncludeColladaDB})

add_dependencies(ColladaDB ChimeraCore)

# Lista os Includes
# get_property(dirs DIRECTORY ${IncludeColladaDB}/chimera/core PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

INSTALL(FILES ${ChimeraCore_BINARY_DIR}/libColladaDB.a DESTINATION lib)
#INSTALL(DIRECTORY ${IncludeColladaDB}/chimera/core DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
