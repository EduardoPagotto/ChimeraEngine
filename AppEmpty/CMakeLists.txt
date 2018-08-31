cmake_minimum_required(VERSION 3.10.2)

PROJECT(AppEmpty)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

#get_target_property(Chimera CHIMERA_INCLUDE_DIR INTERFACE_INCLUDE_DIRECTORIES)

#define os Lib's a serem usados
set (LibsAppEmpty ${Chimera_BINARY_DIR}/libChimera.a
				  ${OPENGL_LIBRARIES}
				  ${SDL2_IBRARIES}
				  ${BULLET_LIBRARIES}
				  ${FREETYPE_LIBRARY}
				  -lSDL2
				  -ltinyxml2
				  -lGLEW
				  -lpthread)

#define os includes
set (IncludesAppEmpty ${CMAKE_CURRENT_SOURCE_DIR}/include
					  ${Chimera_SOURCE_DIR}/include
					  ${OPENGL_INCLUDE_DIR}
					  ${SDL2_INCLUDE_DIR}
					  ${BULLET_INCLUDE_DIR}
					  ${FREETYPE_INCLUDE_DIRS})


if (OVR_SET_TO_USE)
    message(STATUS "OVR Incluido no Projeto")
    set(LibsAppEmpty ${LibsAppEmpty} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable (AppEmpty src/Game.cpp
						 src/AppEmpty.cpp)

target_link_libraries(AppEmpty ${LibsAppEmpty})
target_include_directories (AppEmpty PRIVATE ${IncludesAppEmpty})

add_dependencies(AppEmpty Chimera)

INSTALL(FILES ${AppEmpty_BINARY_DIR}/AppEmpty DESTINATION bin)
