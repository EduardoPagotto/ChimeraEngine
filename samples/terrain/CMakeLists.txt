cmake_minimum_required(VERSION 3.18.2)

PROJECT(Terrain)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)
find_package(PUGIXML REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define os includes
set (IncludesTerrain ${CMAKE_CURRENT_SOURCE_DIR}/include
					 ${PUGIXML_DIR}
					 ${CMAKE_SOURCE_DIR}
					 ${OPENGL_INCLUDE_DIR}
					 ${SDL2_INCLUDE_DIR}
					 ${BULLET_INCLUDE_DIR})

#define os Lib's a serem usados
set (LibsTerrain ${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
				 ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
				 ${OPENGL_LIBRARIES}
				 ${SDL2_IBRARIES}
				 ${BULLET_LIBRARIES}
				 -lGL
				 -lGLU
				 -lglut
				 -lSDL2
				 -lSDL2_image
				 -lSDL2_ttf
				 -lGLEW
				 -lpugixml
				 -lpthread)

add_executable (terrain src/Game.cpp
						src/main.cpp)

target_link_libraries(terrain ${LibsTerrain})
target_include_directories (terrain PRIVATE ${IncludesTerrain})

add_dependencies(terrain ChimeraCore 
						 ChimeraRender)

INSTALL(FILES ${Terrain_BINARY_DIR}/terrain DESTINATION bin)
