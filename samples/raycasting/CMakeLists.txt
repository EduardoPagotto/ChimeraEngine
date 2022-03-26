cmake_minimum_required(VERSION 3.18.2)

PROJECT(RayCasting)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#define os Lib's a serem usados
set (LibsSimpleRayCasting ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
						  ${SDL2_IBRARIES}
						  -lSDL2
						  -lGLEW
						  -lpthread)

#define os includes
set (IncludesSimpleRayCasting ${CMAKE_CURRENT_SOURCE_DIR}/include
							  ${CMAKE_SOURCE_DIR}
							  ${SDL2_INCLUDE_DIR})

add_executable (raycasting src/raycasting.cpp 
						   src/Game.cpp 
						   src/main.cpp)

target_link_libraries(raycasting ${LibsSimpleRayCasting})
target_include_directories (raycasting PRIVATE ${IncludesSimpleRayCasting})

add_dependencies(raycasting ChimeraCore)

INSTALL(FILES ${SimpleRayCasting_BINARY_DIR}/raycasting DESTINATION bin)
