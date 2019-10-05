cmake_minimum_required(VERSION 3.10.2)

PROJECT(TestLoader)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

set (IncludesTestLoader ${CMAKE_SOURCE_DIR}
						${OPENGL_INCLUDE_DIR}
						${SDL2_INCLUDE_DIR}
						${BULLET_INCLUDE_DIR}
						${FREETYPE_INCLUDE_DIRS})

set (LibsTestLoader 	${ChimeraLoaders_BINARY_DIR}/libChimeraLoaders.a #ordem é importante!!!
						${ChimeraNode_BINARY_DIR}/libChimeraNode.a
						${ChimeraCore_BINARY_DIR}/libChimeraCore.a
						${OPENGL_LIBRARIES}
						${SDL2_LIBRARY}
						${BULLET_LIBRARIES}
						${FREETYPE_LIBRARY}
						${GLEW_LIBRARIES}
						-lSDL2
						-lSDL2_image
						-lSDL2_ttf
						-ltinyxml2
						-lpthread
						-lyaml-cpp)

if (OVR_SET_TO_USE)
	message(STATUS "OVR Incluido no Projeto")
	set(IncludesTestLoader ${IncludesTestLoader} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
    set(LibsTestLoader ${LibsTestLoader} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable(TestLoader TestLoader.cpp)

target_include_directories(TestLoader PRIVATE ${IncludesTestLoader})
target_link_libraries(TestLoader ${LibsTestLoader})

add_dependencies(TestLoader ChimeraCore
							ChimeraLoaders)

INSTALL(FILES ${TestLoader_BINARY_DIR}/TestLoader DESTINATION bin)
