cmake_minimum_required(VERSION 2.8)

PROJECT(AppShader)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(
	${AppShader_SOURCE_DIR}/include/
	${Chimera_SOURCE_DIR}/include
	${OPENGL_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR})

add_executable(
	AppShader
	src/GameEvents.cpp
	src/AppShader.cpp)

target_link_libraries(
	AppShader
	${Chimera_BINARY_DIR}/libChimera.a
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARY}
	/usr/lib64/libGLEW.so
	/usr/lib64/libGLEWmx.so
	librt.so
	libXrandr.so
)

#add_dependencies(AppShader chimera)

INSTALL(FILES ${AppShader_BINARY_DIR}/AppShader DESTINATION bin)
