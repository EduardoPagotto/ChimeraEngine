PROJECT(raycasting LANGUAGES CXX)

find_package(SDL3 REQUIRED)

set (IncludeDirs ${CMAKE_CURRENT_SOURCE_DIR}/include
    			 ${CMAKE_BINARY_DIR}/configured_files/include
				 ${CMAKE_SOURCE_DIR})

set (SourceFiles src/raycasting.cpp
				 src/Game.cpp
				 src/main.cpp)

set (LIBRARY_DEPS -lSDL3
				  -lpthread)

add_executable (${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME} ${LIBRARY_DEPS})
target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})

INSTALL(FILES ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} DESTINATION bin)
