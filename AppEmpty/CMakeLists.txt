cmake_minimum_required(VERSION 2.8)

PROJECT(AppEmpty)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
#find_package(LibXml2 REQUIRED)
#find_package(Bullet REQUIRED)
#find_package(Freetype REQUIRED)
#find_package(FTGL REQUIRED)
#find_package(Log4Cxx REQUIRED)

include_directories(
	${AppEmpty_SOURCE_DIR}/include/
	${Chimera_SOURCE_DIR}/include
	${OPENGL_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
#	${LIBXML2_INCLUDE_DIR}
#	${BULLET_INCLUDE_DIR}
#	${FREETYPE_INCLUDE_DIRS}
#	${FTGL_INCLUDE_DIR}
#	${LOG4CXX_INCLUDE_DIR}
#	/usr/local/ovr_sdk_linux_0.4.4/LibOVR
)

add_executable(
	AppEmpty
	src/GameEvents.cpp
	src/AppEmpty.cpp
)

target_link_libraries(
	AppEmpty
	${Chimera_BINARY_DIR}/libChimera.a
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARY}
#	/usr/lib64/libtinyxml2.so
#	${BULLET_LIBRARIES}
#	${FREETYPE_LIBRARY}
##	${FTGL_LIBRARY}
#	${LOG4CXX_LIBRARIES}
#	-lSDL2_image
#	-lSDL2_ttf
	/usr/lib64/libGLEW.so
	/usr/lib64/libGLEWmx.so
	librt.so
	libXrandr.so
#	/usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a
)

#add_dependencies(AppEmpty chimera)

INSTALL(FILES ${AppEmpty_BINARY_DIR}/AppEmpty DESTINATION bin)
