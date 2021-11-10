cmake_minimum_required(VERSION 3.10.2)

PROJECT(Models2)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set (IncludesModels ${Models_SOURCE_DIR}/include/
					${CMAKE_SOURCE_DIR}
					${OPENGL_INCLUDE_DIR}
					${SDL2_INCLUDE_DIR}
					${BULLET_INCLUDE_DIR})

#ordem é importante!!!
set (LibsModels ${CMAKE_SOURCE_DIR}/bin/libChimeraPhysicLoader.a
				${CMAKE_SOURCE_DIR}/bin/libChimeraLoaders.a
				${CMAKE_SOURCE_DIR}/bin/libChimeraPhysic.a
				${CMAKE_SOURCE_DIR}/bin/libChimeraNode.a
				${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
				${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
				${OPENGL_LIBRARIES}
				${SDL2_LIBRARY}
				${BULLET_LIBRARIES}
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

add_executable(models2 src/Game.cpp
					  src/main.cpp)

target_include_directories(models2 PRIVATE ${IncludesModels})
target_link_libraries(models2 ${LibsModels})

add_dependencies(models2 ChimeraCore
						ChimeraRender
						ChimeraNode
						ChimeraPhysic
						ChimeraLoaders
						ChimeraPhysicLoader)

INSTALL(FILES ${Models_BINARY_DIR}/models2 DESTINATION bin)
