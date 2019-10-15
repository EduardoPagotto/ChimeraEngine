cmake_minimum_required(VERSION 3.10.2)

PROJECT(Shader)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

set (IncludesAppShare ${Shader_SOURCE_DIR}/include/
					  ${CMAKE_SOURCE_DIR}
					  ${OPENGL_INCLUDE_DIR}
					  ${SDL2_INCLUDE_DIR}
					  ${BULLET_INCLUDE_DIR}
					  ${FREETYPE_INCLUDE_DIRS})

set (LibsAppShader ${CMAKE_SOURCE_DIR}/bin/libChimeraLoaders.a
				   ${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
				   ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
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

add_executable(shader src/Game.cpp
					  src/main.cpp)

target_link_libraries(shader ${LibsAppShader})
target_include_directories (shader PRIVATE ${IncludesAppShare})

add_dependencies(shader ChimeraCore
						ChimeraRender
						ChimeraLoaders)

INSTALL(FILES ${AppShader_BINARY_DIR}/shader DESTINATION bin)
