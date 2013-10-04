cmake_minimum_required(VERSION 2.8)

PROJECT(appTest)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package (LibXml2 REQUIRED)

if (LIBXML2_FOUND)
	add_definitions( ${LIBXML2_DEFINITIONS} )
	include_directories(${LIBXML2_INCLUDE_DIR})
	link_libraries (${LIBXML2_LIBRARIES})
endif (LIBXML2_FOUND)

find_package (Log4Cxx REQUIRED)
if (Log4Cxx_FOUND)
	add_definitions( ${Log4Cxx_DEFINITIONS} )
	include_directories(${Log4Cxx_INCLUDE_DIR})
	link_libraries (${Log4Cxx_LIBRARIES})	
	endif (Log4Cxx_FOUND)

include_directories(include ../chimera/include/ ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${BULLET_INCLUDE_DIR})

link_libraries(/usr/lib64/liblog4cxx.so)

link_libraries(${Chimera_BINARY_DIR}/libChimera.a)
link_libraries(${OPENGL_LIBRARIES})
link_libraries(${SDL2_LIBRARIES})
link_libraries(${BULLET_LIBRARIES})
link_libraries(-lSDL2 -lSDL2_image)

add_executable(appTest
        src/Game.cpp
        src/appTest.cpp)

add_dependencies(appTest chimera)

INSTALL(FILES ${appTest_BINARY_DIR}/appTest DESTINATION bin)
