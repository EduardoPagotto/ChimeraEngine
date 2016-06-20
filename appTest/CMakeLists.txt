cmake_minimum_required(VERSION 2.8)

PROJECT(appTest)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
#find_package(LibXml2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)	
find_package(Log4Cxx REQUIRED)

include_directories(
	${AppCargaManual_SOURCE_DIR}/include/
	${Chimera_SOURCE_DIR}/include
	${OPENGL_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
#	${LIBXML2_INCLUDE_DIR}
	${BULLET_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${FTGL_INCLUDE_DIR}
	${LOG4CXX_INCLUDE_DIR}
	/usr/local/ovr_sdk_linux_0.4.4/LibOVR
)

add_executable(
	appTest
	${appTest_SOURCE_DIR}/src/Game.cpp
	${appTest_SOURCE_DIR}/src/appTest.cpp
)

target_link_libraries(
	appTest 
	${Chimera_BINARY_DIR}/libChimera.a
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARY}
#	${LIBXML2_LIBRARIES}
	#/usr/local/lib64/libtinyxml2.a
	/usr/lib64/libtinyxml2.so
	${BULLET_LIBRARIES}
	${FREETYPE_LIBRARY}
	${FTGL_LIBRARY}
	${LOG4CXX_LIBRARIES}
	-lSDL2_image
	-lSDL2_ttf
	/usr/lib64/libGLEW.so
	/usr/lib64/libGLEWmx.so
	librt.so
	libXrandr.so
	/usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a
	#libGL.so
	#libXxf86vm.so
	#libpthread.so
	#${UDEV_LIBRARIES}
	#${XINERAMA_LIBRARY}
	#${X11_LIBRARIES}
)

#add_dependencies(appTest chimera)

INSTALL(FILES ${appTest_BINARY_DIR}/appTest DESTINATION bin)
