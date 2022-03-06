cmake_minimum_required(VERSION 3.18.4)

PROJECT(LoadTest)

find_package(SDL2 REQUIRED)

set (IncludesLoad	${CMAKE_SOURCE_DIR}
					${SDL2_INCLUDE_DIR})

set (LibsLoad 		-lSDL2
					-lpthread)

add_executable(loadtest main.cpp)

target_include_directories(loadtest PRIVATE ${IncludesLoad})
target_link_libraries(loadtest ${LibsLoad})

# add_dependencies(loadtest ChimeraCore
# 						ChimeraRender
# 						ChimeraLoaders)

INSTALL(FILES ${Models2_BINARY_DIR}/loadtest DESTINATION bin)
