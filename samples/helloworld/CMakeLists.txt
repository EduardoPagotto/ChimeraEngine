PROJECT(helloworld LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)
find_package(PUGIXML REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define os includes
set (IncludeDirs ${CMAKE_SOURCE_DIR}
                 ${CMAKE_BINARY_DIR}/configured_files/include
				 ${PUGIXML_DIR}
				 ${OPENGL_INCLUDE_DIR}
				 ${SDL3_INCLUDE_DIR}
				 ${BULLET_INCLUDE_DIR})

set (SourceFiles TileLayer.cpp
                 Game.cpp
				 main.cpp)

#define os Lib's a serem usados
set (LIBRARY_DEPS ${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
				  ${CMAKE_SOURCE_DIR}/bin/libChimeraCollada.a
				  ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
				  ${OPENGL_LIBRARIES}
				  ${SDL3_IBRARIES}
				  ${BULLET_LIBRARIES}
				  -lGL
				  -lSDL3
				  -lSDL3_image
				  -lSDL3_ttf
				  -lGLEW
				  -lpugixml
				  -lpthread)

add_executable (${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME} ${LIBRARY_DEPS})
target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})

add_dependencies(${PROJECT_NAME} ChimeraCore
							     ChimeraRender)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} DESTINATION bin)
