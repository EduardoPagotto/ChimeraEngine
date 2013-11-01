cmake_minimum_required(VERSION 2.8)

PROJECT(AppCargaManual)

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
)

add_executable(
	AppCargaManual
	src/Game.cpp
	src/main.cpp
)

target_link_libraries(
	AppCargaManual 
	${Chimera_BINARY_DIR}/libChimera.a 
	${OPENGL_LIBRARIES} 
	${SDL2_LIBRARY} 
#	${LIBXML2_LIBRARIES}
	/usr/local/lib64/libtinyxml2.a
	${BULLET_LIBRARIES} 
	${FREETYPE_LIBRARY} 
	${FTGL_LIBRARY} 
	${LOG4CXX_LIBRARIES} 
	-lSDL2_image
)

add_dependencies(AppCargaManual chimera)

INSTALL(FILES ${appTest2_BINARY_DIR}/AppCargaManual DESTINATION bin)
