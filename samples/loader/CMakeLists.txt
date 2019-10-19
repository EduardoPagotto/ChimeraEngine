cmake_minimum_required(VERSION 3.10.2)

PROJECT(Loader)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set (IncludesLoader ${CMAKE_SOURCE_DIR}
					${OPENGL_INCLUDE_DIR}
					${SDL2_INCLUDE_DIR}
					${BULLET_INCLUDE_DIR}
					${FREETYPE_INCLUDE_DIRS})

set (LibsLoader ${CMAKE_SOURCE_DIR}/bin/libChimeraLoaders.a #ordem é importante!!!
				${CMAKE_SOURCE_DIR}/bin/libChimeraNode.a
				${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
				${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
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
	set(IncludesLoader ${IncludesLoader} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
    set(LibsLoader ${LibsLoader} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable(loader main.cpp)

target_include_directories(loader PRIVATE ${IncludesLoader})
target_link_libraries(loader ${LibsLoader})

add_dependencies(loader ChimeraCore
						ChimeraRender
						ChimeraNode
						ChimeraLoaders)

INSTALL(FILES ${Loader_BINARY_DIR}/loader DESTINATION bin)
