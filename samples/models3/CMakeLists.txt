cmake_minimum_required(VERSION 3.18.2)

PROJECT(Models3)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)
find_package(PUGIXML REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set (IncludesModels3 ${Models3_SOURCE_DIR}/include/
					 ${PUGIXML_DIR}
					 ${CMAKE_SOURCE_DIR}
					 ${OPENGL_INCLUDE_DIR}
					 ${SDL2_INCLUDE_DIR}
					 ${BULLET_INCLUDE_DIR})

#ordem é importante!!!
set (LibsModels3 ${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
				 ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
				 ${OPENGL_LIBRARIES}
				 ${SDL2_LIBRARY}
				 ${BULLET_LIBRARIES}
				 ${GLEW_LIBRARIES}
				 -lSDL2
				 -lSDL2_image
				 -lSDL2_ttf
				 -lpugixml
				 -lpthread)

if (OVR_SET_TO_USE)
	message(STATUS "OVR Incluido no Projeto")
	set(IncludesModels3 ${IncludesModels3} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
    set(LibsModels3 ${LibsModels3} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable(models3 	src/Game.cpp
					  	src/main.cpp)

target_include_directories(models3 PRIVATE ${IncludesModels3})
target_link_libraries(models3 ${LibsModels3})

add_dependencies(models3 ChimeraCore
						 ChimeraRender)

INSTALL(FILES ${Models3_BINARY_DIR}/models3 DESTINATION bin)
