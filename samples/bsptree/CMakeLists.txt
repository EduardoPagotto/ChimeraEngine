cmake_minimum_required(VERSION 3.10.2)

PROJECT(BSTree)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

#define os Lib's a serem usados
set (LibsTesteBSTree ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
					 ${OPENGL_LIBRARIES}
					 ${SDL2_IBRARIES}
					 -lGL
					 -lGLU
					 -lglut
					 -lSDL2
					 -lSDL2_image
					 -lGLEW
					 -lpthread)

#define os includes
set (IncludesTesteBSTree ${CMAKE_CURRENT_SOURCE_DIR}/include
						 ${CMAKE_SOURCE_DIR}
						 ${OPENGL_INCLUDE_DIR}
						 ${SDL2_INCLUDE_DIR})

add_executable (bsptree src/Triangle.cpp
						src/BSPTree.cpp
						src/BSPTreeBuilder.cpp
						src/Game.cpp 
						src/main.cpp)

target_link_libraries(bsptree ${LibsTesteBSTree})
target_include_directories (bsptree PRIVATE ${IncludesTesteBSTree})

add_dependencies(bsptree ChimeraCore)

INSTALL(FILES ${TesteBSTree_BINARY_DIR}/bsptree DESTINATION bin)
