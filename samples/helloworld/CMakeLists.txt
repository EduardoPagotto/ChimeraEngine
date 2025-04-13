cmake_minimum_required(VERSION 3.18.2)

PROJECT(HelloWorld)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)
find_package(PUGIXML REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define os Lib's a serem usados
set (LibsHelloWorld ${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
					${CMAKE_SOURCE_DIR}/bin/libChimeraCollada.a
					${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
				    ${OPENGL_LIBRARIES}
				    ${SDL2_IBRARIES}
					${BULLET_LIBRARIES}
					-lGL
					-lSDL2
					-lSDL2_image
					-lSDL2_ttf
				    -lGLEW
					-lpugixml
				    -lpthread)

#define os includes
set (IncludesHelloWorld ${CMAKE_SOURCE_DIR}
						${PUGIXML_DIR}
						${OPENGL_INCLUDE_DIR}
						${SDL2_INCLUDE_DIR}
						${BULLET_INCLUDE_DIR})

add_executable (helloworld TileLayer.cpp Game.cpp main.cpp)

target_link_libraries(helloworld ${LibsHelloWorld})
target_include_directories (helloworld PRIVATE ${IncludesHelloWorld})

add_dependencies(helloworld ChimeraCore 
							ChimeraRender)

INSTALL(FILES ${HelloWorld_BINARY_DIR}/helloworld DESTINATION bin)
