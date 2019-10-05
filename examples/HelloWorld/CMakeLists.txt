cmake_minimum_required(VERSION 3.10.2)

PROJECT(HelloWorld)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

#get_target_property(Chimera CHIMERA_INCLUDE_DIR INTERFACE_INCLUDE_DIRECTORIES)

#define os Lib's a serem usados
set (LibsHelloWorld ${ChimeraCore_BINARY_DIR}/libChimeraCore.a
				  ${OPENGL_LIBRARIES}
				  ${SDL2_IBRARIES}
				  ${BULLET_LIBRARIES}
				  ${FREETYPE_LIBRARY}
				  -lSDL2
				  -ltinyxml2
				  -lGLEW
				  -lpthread
				  -lyaml-cpp)

#define os includes
set (IncludesHelloWorld ${CMAKE_CURRENT_SOURCE_DIR}/include
						${CMAKE_SOURCE_DIR}
						${OPENGL_INCLUDE_DIR}
						${SDL2_INCLUDE_DIR}
						${BULLET_INCLUDE_DIR}
						${FREETYPE_INCLUDE_DIRS})


if (OVR_SET_TO_USE)
    message(STATUS "OVR Incluido no Projeto")
    set(LibsHelloWorld ${LibsHelloWorld} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable (HelloWorld src/Game.cpp 
						   src/main.cpp)

target_link_libraries(HelloWorld ${LibsHelloWorld})
target_include_directories (HelloWorld PRIVATE ${IncludesHelloWorld})

add_dependencies(HelloWorld ChimeraCore)

INSTALL(FILES ${HelloWorld_BINARY_DIR}/HelloWorld DESTINATION bin)
