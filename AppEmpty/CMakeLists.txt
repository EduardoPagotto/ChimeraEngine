cmake_minimum_required(VERSION 2.8)

PROJECT(AppEmpty)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)

include_directories(
	${AppEmpty_SOURCE_DIR}/include/
	${Chimera_SOURCE_DIR}/include
	${OPENGL_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	${BULLET_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${FTGL_INCLUDE_DIR}
#	${LOG4CXX_INCLUDE_DIR}
)
add_executable(
	AppEmpty
	src/Game.cpp
	src/AppEmpty.cpp
)

target_link_libraries(
	AppEmpty
	${Chimera_BINARY_DIR}/libChimera.a
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARY}
	/usr/lib64/libGLEW.so
	/usr/lib64/libGLEWmx.so
	librt.so
	libXrandr.so)

set (SourcesChimeraAPPEmpty 	
	${Chimera_BINARY_DIR}/libChimera.a
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARY}
	${BULLET_LIBRARIES}
	${FREETYPE_LIBRARY}
	${FTGL_LIBRARY}
	#${LOG4CXX_LIBRARIES}
	-lSDL2
	-lSDL2_image
	-lSDL2_ttf
	-ltinyxml2
	-lGLEW
	-lGLEWmx
	librt.so
	libXrandr.so
	-lpthread
)

if (OVR_SET_TO_USE)
    message(STATUS "OVR Incluido no Projeto")
    set(SourcesChimeraAPPEmpty ${SourcesChimeraAPPEmpty} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

target_link_libraries(AppEmpty ${SourcesChimeraAPPEmpty})

INSTALL(FILES ${AppEmpty_BINARY_DIR}/AppEmpty DESTINATION bin)
