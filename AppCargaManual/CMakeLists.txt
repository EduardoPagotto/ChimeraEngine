cmake_minimum_required(VERSION 3.5.2)

PROJECT(AppCargaManual)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)
find_package(GLEW REQUIRED)
#find_package(Log4Cxx REQUIRED)

include_directories(
	../../chimera/include
	${AppCargaManual_SOURCE_DIR}/include/
	${Chimera_SOURCE_DIR}/include
	${OPENGL_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	${BULLET_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
#	${FTGL_INCLUDE_DIR}
#	${LOG4CXX_INCLUDE_DIR}
)

if (OVR_SET_TO_USE)
  include_directories(/usr/local/ovr_sdk_linux_0.4.4/LibOVR)
endif(OVR_SET_TO_USE)

add_executable(
	AppCargaManual
	src/Game.cpp
	src/main.cpp)

set (SourcesChimeraAPPCargaManual
	${Chimera_BINARY_DIR}/libChimera.a
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARY}
	${BULLET_LIBRARIES}
	${FREETYPE_LIBRARY}
	${FTGL_LIBRARY}
	${GLEW_LIBRARIES}
	#${LOG4CXX_LIBRARIES}
	-lSDL2
	-lSDL2_image
	-lSDL2_ttf
	-ltinyxml2
	-lGLEW
	#-lGLEWmx
	librt.so
	libXrandr.so
	-lpthread
)

if (OVR_SET_TO_USE)
    message(STATUS "OVR Incluido no Projeto")
    set(SourcesChimeraAPPCargaManual ${SourcesChimeraAPPCargaManual} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

target_link_libraries(AppCargaManual ${SourcesChimeraAPPCargaManual})

add_dependencies(AppCargaManual Chimera)

INSTALL(FILES ${AppCargaManual_BINARY_DIR}/AppCargaManual DESTINATION bin)
