cmake_minimum_required(VERSION 3.18.4)

PROJECT(LoadTest)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PUGIXML REQUIRED) #PUGIXML
find_package(GLEW REQUIRED)

set (IncludesLoad	${CMAKE_SOURCE_DIR}
					${PUGIXML_DIR}
					${SDL2_INCLUDE_DIR})

set (LibsLoad 		${CMAKE_SOURCE_DIR}/bin/libColladaDB.a
					${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
					-lGL
					-lpugixml
					-lSDL2
					-lSDL2_image
					-lGLEW
					-lpthread)

add_executable(loadtest main.cpp)

target_include_directories(loadtest PRIVATE ${IncludesLoad})
target_link_libraries(loadtest ${LibsLoad})

add_dependencies(loadtest ColladaDB)
# 						ChimeraRender
# 						ChimeraLoaders)

INSTALL(FILES ${Models2_BINARY_DIR}/loadtest DESTINATION bin)
