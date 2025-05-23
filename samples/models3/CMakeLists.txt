PROJECT(models3 LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)
find_package(PUGIXML REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set (IncludeDirs ${CMAKE_CURRENT_SOURCE_DIR}/include
					 ${PUGIXML_DIR}
					 ${CMAKE_SOURCE_DIR}
					 ${OPENGL_INCLUDE_DIR}
					 ${SDL2_INCLUDE_DIR}
					 ${BULLET_INCLUDE_DIR})

set (SourceFiles src/Game.cpp
				 src/main.cpp)

#ordem é importante!!!
set (LIBRARY_DEPS ${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
				 ${CMAKE_SOURCE_DIR}/bin/libChimeraCollada.a
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
	set(IncludeDirs ${IncludeDirs} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
    set(LIBRARY_DEPS ${LIBRARY_DEPS} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable (${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME} ${LIBRARY_DEPS})

target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})

add_dependencies(${PROJECT_NAME} ChimeraCore
						         ChimeraRender)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} DESTINATION bin)
