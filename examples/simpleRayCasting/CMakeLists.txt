cmake_minimum_required(VERSION 3.10.2)

PROJECT(SimpleRayCasting)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

#define os Lib's a serem usados
set (LibsSimpleRayCasting ${ChimeraCore_BINARY_DIR}/libChimeraCore.a
				  ${SDL2_IBRARIES}
				  -lSDL2
				  -lGLEW
				  -lpthread)

#define os includes
set (IncludesSimpleRayCasting ${CMAKE_CURRENT_SOURCE_DIR}/include
					  ${CMAKE_SOURCE_DIR}/libs
					  ${SDL2_INCLUDE_DIR})

add_executable (SimpleRayCasting raycasting.cpp 
								 Game.cpp 
								 main.cpp)

target_link_libraries(SimpleRayCasting ${LibsSimpleRayCasting})
target_include_directories (SimpleRayCasting PRIVATE ${IncludesSimpleRayCasting})

add_dependencies(SimpleRayCasting ChimeraCore)

INSTALL(FILES ${SimpleRayCasting_BINARY_DIR}/SimpleRayCasting DESTINATION bin)
