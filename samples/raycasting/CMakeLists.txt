cmake_minimum_required(VERSION 3.18.2)

PROJECT(RayCasting)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Bullet REQUIRED)
find_package(PUGIXML REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set (IncludesSimpleRayCasting ${CMAKE_CURRENT_SOURCE_DIR}/include
							  ${PUGIXML_DIR}
							  ${CMAKE_SOURCE_DIR}
							  ${OPENGL_INCLUDE_DIR}
							  ${SDL2_INCLUDE_DIR}
							  ${BULLET_INCLUDE_DIR})

set (LibsSimpleRayCasting ${CMAKE_SOURCE_DIR}/bin/libChimeraCollada.a
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

add_executable (raycasting src/raycasting.cpp 
						   src/Game.cpp 
						   src/main.cpp)

target_link_libraries(raycasting ${LibsSimpleRayCasting})
target_include_directories (raycasting PRIVATE ${IncludesSimpleRayCasting})

add_dependencies(raycasting ChimeraCollada)

INSTALL(FILES ${SimpleRayCasting_BINARY_DIR}/raycasting DESTINATION bin)
