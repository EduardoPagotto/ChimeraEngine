cmake_minimum_required(VERSION 3.10.2)

PROJECT(Simples)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define os Lib's a serem usados
set (LibsSimples ${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
                 ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
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
set (IncludesSimples ${CMAKE_CURRENT_SOURCE_DIR}/include
						 ${CMAKE_SOURCE_DIR}
						 ${OPENGL_INCLUDE_DIR}
						 ${SDL2_INCLUDE_DIR})

add_executable (simples src/Game.cpp 
						src/main.cpp)

target_link_libraries(simples ${LibsSimples})
target_include_directories (simples PRIVATE ${IncludesSimples})

add_dependencies(simples ChimeraCore
                         ChimeraRender)

INSTALL(FILES ${Simples_BINARY_DIR}/simples DESTINATION bin)
