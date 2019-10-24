cmake_minimum_required(VERSION 3.10.2)

PROJECT(HelloWorld)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define os Lib's a serem usados
set (LibsHelloWorld ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
				  ${OPENGL_LIBRARIES}
				  ${SDL2_IBRARIES}
				  -lSDL2
				  -lGLEW
				  -lpthread
				  -lyaml-cpp)

#define os includes
set (IncludesHelloWorld ${CMAKE_CURRENT_SOURCE_DIR}/include
						${CMAKE_SOURCE_DIR}
						${OPENGL_INCLUDE_DIR}
						${SDL2_INCLUDE_DIR})

add_executable (helloworld src/Game.cpp 
						   src/main.cpp)

target_link_libraries(helloworld ${LibsHelloWorld})
target_include_directories (helloworld PRIVATE ${IncludesHelloWorld})

add_dependencies(helloworld ChimeraCore)

INSTALL(FILES ${HelloWorld_BINARY_DIR}/helloworld DESTINATION bin)
