cmake_minimum_required(VERSION 3.10.2)

PROJECT(TesteBSTree)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

#define os Lib's a serem usados
set (LibsTesteBSTree ${ChimeraCore_BINARY_DIR}/libChimeraCore.a
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

add_executable (TesteBSTree Triangle.cpp
							BSPTree.cpp
							BSPTreeBuilder.cpp
							Game.cpp 
							main.cpp)

target_link_libraries(TesteBSTree ${LibsTesteBSTree})
target_include_directories (TesteBSTree PRIVATE ${IncludesTesteBSTree})

add_dependencies(TesteBSTree ChimeraCore)

INSTALL(FILES ${TesteBSTree_BINARY_DIR}/TesteBSTree DESTINATION bin)
