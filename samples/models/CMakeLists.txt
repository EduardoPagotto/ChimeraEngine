cmake_minimum_required(VERSION 3.10.2)

PROJECT(Models)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

set (IncludesModels ${Models_SOURCE_DIR}/include/
					${CMAKE_SOURCE_DIR}
					${OPENGL_INCLUDE_DIR}
					${SDL2_INCLUDE_DIR}
					${BULLET_INCLUDE_DIR}
					${FREETYPE_INCLUDE_DIRS})

set (LibsModels ${CMAKE_SOURCE_DIR}/bin/libChimeraLoaders.a #ordem é importante!!!
				${CMAKE_SOURCE_DIR}/bin/libChimeraNode.a
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
	set(IncludesModels ${IncludesModels} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
    set(LibsModels ${LibsModels} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable(models src/Game.cpp
					  src/main.cpp)

target_include_directories(models PRIVATE ${IncludesModels})
target_link_libraries(models ${LibsModels})

add_dependencies(models ChimeraCore
						ChimeraNode
						ChimeraLoaders)

INSTALL(FILES ${Models_BINARY_DIR}/models DESTINATION bin)
