cmake_minimum_required(VERSION 3.10.2)

PROJECT(appTest)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

set (IncludesAppTest ${appTest_SOURCE_DIR}/include/
					 ${CMAKE_SOURCE_DIR}
					 ${OPENGL_INCLUDE_DIR}
					 ${SDL2_INCLUDE_DIR}
					 ${BULLET_INCLUDE_DIR}
					 ${FREETYPE_INCLUDE_DIRS}
					 ${GLEW_INCLUDE_DIR})

set (LibsAppTest ${CMAKE_SOURCE_DIR}/bin/libChimeraLoaders.a #ordem é importante!!!
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

add_executable(appTest src/Game.cpp
					   src/appTest.cpp)

if (OVR_SET_TO_USE)
    message(STATUS "OVR Incluido no Projeto")
    set(LibsAppTest ${LibsAppTest} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)


target_include_directories (appTest PRIVATE ${IncludesAppTest})
target_link_libraries(appTest ${LibsAppTest})

add_dependencies(appTest ChimeraCore
						 ChimeraNode
						 ChimeraLoaders)

INSTALL(FILES ${appTest_BINARY_DIR}/appTest DESTINATION bin)
