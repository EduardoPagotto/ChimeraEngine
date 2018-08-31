cmake_minimum_required(VERSION 3.10.2)

PROJECT(AppCargaManual)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

set (IncludesAppCargaManual ${AppCargaManual_SOURCE_DIR}/include/
							${Chimera_SOURCE_DIR}/include
							${OPENGL_INCLUDE_DIR}
							${SDL2_INCLUDE_DIR}
							${BULLET_INCLUDE_DIR}
							${FREETYPE_INCLUDE_DIRS})

set (LibsAppCargaManual ${Chimera_BINARY_DIR}/libChimera.a
						${OPENGL_LIBRARIES}
						${SDL2_LIBRARY}
						${BULLET_LIBRARIES}
						${FREETYPE_LIBRARY}
						${GLEW_LIBRARIES}
						-lSDL2
						-lSDL2_image
						-lSDL2_ttf
						-ltinyxml2
						-lpthread)

if (OVR_SET_TO_USE)
	message(STATUS "OVR Incluido no Projeto")
	set(IncludesAppCargaManual ${IncludesAppCargaManual} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
    set(LibsAppCargaManual ${LibsAppCargaManual} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable(AppCargaManual src/Game.cpp
							  src/main.cpp)

target_include_directories(AppCargaManual PRIVATE ${IncludesAppCargaManual})
target_link_libraries(AppCargaManual ${LibsAppCargaManual})

add_dependencies(AppCargaManual Chimera)

INSTALL(FILES ${AppCargaManual_BINARY_DIR}/AppCargaManual DESTINATION bin)
