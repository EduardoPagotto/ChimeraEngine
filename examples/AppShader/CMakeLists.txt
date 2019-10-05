cmake_minimum_required(VERSION 3.10.2)

PROJECT(AppShader)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

set (IncludesAppShare ${AppShader_SOURCE_DIR}/include/
					  ${CMAKE_SOURCE_DIR}
					  ${OPENGL_INCLUDE_DIR}
					  ${SDL2_INCLUDE_DIR}
					  ${BULLET_INCLUDE_DIR}
					  ${FREETYPE_INCLUDE_DIRS})

set (LibsAppShader ${ChimeraLoaders_BINARY_DIR}/libChimeraLoaders.a
				   ${ChimeraCore_BINARY_DIR}/libChimeraCore.a
				   ${OPENGL_LIBRARIES}
				   ${SDL2_LIBRARY}
				   ${BULLET_LIBRARIES}
				   ${FREETYPE_LIBRARY}
				   ${GLEW_LIBRARY}
				   -lSDL2
				   -lSDL2_image
				   -lSDL2_ttf
				   -ltinyxml2
				   -lpthread
				   -lyaml-cpp)

if (OVR_SET_TO_USE)
    message(STATUS "OVR Incluido no Projeto")
    set(LibsAppShader ${LibsAppShader} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable(AppShader src/Game.cpp
						 src/AppShader.cpp)

target_link_libraries(AppShader ${LibsAppShader})
target_include_directories (AppShader PRIVATE ${IncludesAppShare})

add_dependencies(AppShader ChimeraCore
						   ChimeraLoaders)

INSTALL(FILES ${AppShader_BINARY_DIR}/AppShader DESTINATION bin)
